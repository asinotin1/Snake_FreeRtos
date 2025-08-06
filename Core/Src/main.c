#include "main.h"
//----------------------CONFIG---------------------------------------------
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       160

#define MAX_SNAKE_LENGTH    20 // MAX FOR SNAKE

#define DIR_EVENT_UP        (1<<0)
#define DIR_EVENT_DOWN      (1<<1)
#define DIR_EVENT_LEFT      (1<<2)
#define DIR_EVENT_RIGHT     (1<<3)

//------------------------------STRUCT------------------------------------------

typedef enum
{
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
}Direction;

typedef struct
{
  uint8_t x;
  uint8_t y;
}Point;


typedef struct 
{
  Point body[MAX_SNAKE_LENGTH];
  uint8_t length;
  Direction Dir;
}Snake;




  Snake snake;
  Point food;

  SemaphoreHandle_t xSnakeMutex; //  takes control of the snake
  SemaphoreHandle_t xLcdMutex;   //  takes control of the lcd
  EventGroupHandle_t xDirectionevent;

  volatile int Growing = 0; // check Snake eat food

  void SafeDrawPixel(uint8_t x, uint8_t y,uint16_t color )
  {
    xSemaphoreTake(xLcdMutex,portMAX_DELAY);// check lcd

    drawPixel(x,y,color);
    xSemaphoreGive(xLcdMutex); // release
    
  }

  void DrawPixelBlock(uint8_t x, uint8_t y,uint16_t color)
  {
    for(int dx = 0 ;dx<2;dx++)
    {
      for(int dy = 0;dy<2;dy++)
      {
        SafeDrawPixel(x*2 +dx ,y*2 + dy,color);
      }
    }
  }
  void Drawfood()
  {
    DrawPixelBlock(food.x,food.y,0xF800);
  }

  void DrawHead(Point P)
  {
    DrawPixelBlock(P.x,P.y,0x07E0);
  }

  void ClearTail(Point tail)
  {
    DrawPixelBlock(tail.x,tail.y,0x0000);
  }
  
  void GenerateFood()
  {
    food.x = rand() % (SCREEN_WIDTH/2);
    food.y = rand() % (SCREEN_HEIGHT/2);
  }

  void initGame()
  {
    GPIO_Config(GPIOA,GPIO_PIN_0,GPIO_MODE_INPUT_PU);
    GPIO_Config(GPIOA,GPIO_PIN_2,GPIO_MODE_INPUT_PU);
    GPIO_Config(GPIOA,GPIO_PIN_3,GPIO_MODE_INPUT_PU);
    GPIO_Config(GPIOA,GPIO_PIN_4,GPIO_MODE_INPUT_PU);

    snake.length = 3;
    snake.Dir = DIR_RIGHT;
    snake.body[0] = (Point){5,5};
    snake.body[1] = (Point){4,5};
    snake.body[2] = (Point){3,5};

    GenerateFood();
    fullDisplay(0x0000);
    Drawfood();
    DrawHead(snake.body[0]);
    DrawPixelBlock(snake.body[1].x,snake.body[1].y,0x07E0);
    DrawPixelBlock(snake.body[2].x,snake.body[2].y,0x07E0);
    
  }
  

  void checkcollision()
  {
    Point head = snake.body[0];
    // dam tuong
    if(head.x >= SCREEN_WIDTH/2 || head.y >= SCREEN_HEIGHT/2 || head.x < 0 || head.y < 0)
    {
    xSemaphoreTake(xLcdMutex,portMAX_DELAY);
    drawString(0,0,"Hit Wall", Font_7x10,0xFFFF,0x0000);
    xSemaphoreGive(xLcdMutex);
      while(1)
      {

      }
    }

    if(head.x == food.x && head.y == food.y)
    {
      if(snake.length < MAX_SNAKE_LENGTH)
      {
        snake.body[snake.length] = snake.body[snake.length -1];
        snake.length ++;
        Growing = 1;
        GenerateFood();
        Drawfood();
      }else
      {
        Growing = 0;
      }
    }else
      {
        Growing = 0;
      }
  }

  void moveSnake()
  {
    Point tail = snake.body[snake.length -1];
    for(int i = snake.length -1 ; i>0;  i --)
    {
      snake.body[i] = snake.body[i -1];
    }


    switch (snake.Dir)
    {
    case DIR_UP:
      snake.body[0].y-- ;
      break;
    case DIR_DOWN:
      snake.body[0].y++ ;
      break;
    case DIR_LEFT:
      snake.body[0].x-- ;
      break;
    case DIR_RIGHT:
      snake.body[0].x++ ;
      break;
    }


    if(Growing == 0)
    {
      ClearTail(tail);
    }
  }

  void vTaskSnake()
  {
    while(1)
    {
      EventBits_t bits = xEventGroupWaitBits(xDirectionevent,DIR_EVENT_UP | DIR_EVENT_DOWN |
                        DIR_EVENT_LEFT | DIR_EVENT_RIGHT,pdTRUE,pdFALSE,pdMS_TO_TICKS(0));
        xSemaphoreTake(xSnakeMutex,portMAX_DELAY);
        
        if((bits & DIR_EVENT_UP) && snake.Dir != DIR_DOWN)
        {
          snake.Dir = DIR_UP;
        }
        if((bits & DIR_EVENT_DOWN) && snake.Dir != DIR_UP)
        {
          snake.Dir = DIR_DOWN;
        }
        if((bits & DIR_EVENT_LEFT) && snake.Dir != DIR_RIGHT)
        {
          snake.Dir = DIR_LEFT;
        }
        if((bits & DIR_EVENT_RIGHT) && snake.Dir != DIR_LEFT)
        {
          snake.Dir = DIR_RIGHT;
        }

        checkcollision();
        moveSnake();
        DrawHead(snake.body[0]);
        xSemaphoreGive(xSnakeMutex);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
  }

  int Button_Up_pressed()
  {
    return GPIO_Read_Pin(GPIOA,GPIO_PIN_4) == 0;
  }
   int Button_Down_pressed()
  {
    return GPIO_Read_Pin(GPIOA,GPIO_PIN_0) == 0;
  }
   int Button_Left_pressed()
  {
    return GPIO_Read_Pin(GPIOA,GPIO_PIN_2) == 0;
  }
   int Button_Right_pressed()
  {
    return GPIO_Read_Pin(GPIOA,GPIO_PIN_3) == 0;
  }


  void vTaskInput()
  {
    while(1)
    {
      if(Button_Up_pressed())
      {
        xEventGroupSetBits(xDirectionevent,DIR_EVENT_UP);
      }
       if(Button_Down_pressed())
      {
        xEventGroupSetBits(xDirectionevent,DIR_EVENT_DOWN);
      }
       if(Button_Left_pressed())
      {
        xEventGroupSetBits(xDirectionevent,DIR_EVENT_LEFT);
      }
       if(Button_Right_pressed())
      {
        xEventGroupSetBits(xDirectionevent,DIR_EVENT_RIGHT);
      }

      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }

int main()
{
  
  RCC_Enable_PortA();
  RCC_Enable_PortB();
  RCC_Enable_AFIO();
  RCC_Enable_TIM2();
  RCC_Enable_SPI1();

  SPI1_Init_Master();
  initTFT();
  fullDisplay(0x0000);
  
  xSnakeMutex = xSemaphoreCreateMutex();
  xLcdMutex = xSemaphoreCreateMutex();
  xDirectionevent = xEventGroupCreate();

  initGame();
  
  if(xTaskCreate(vTaskSnake,"Snake",256,NULL,2,NULL) != pdPASS || xTaskCreate(vTaskInput,"Input",128,NULL,2,NULL) != pdPASS)
  {
    xSemaphoreTake(xLcdMutex,portMAX_DELAY);
    drawString(0,0,"Task Fail", Font_7x10,0xFFFF,0x0000);
    xSemaphoreGive(xLcdMutex);
    while(1)
    {

    }
  }

  vTaskStartScheduler();
  while (1)
  {
    
  }
  
  return 0;
}
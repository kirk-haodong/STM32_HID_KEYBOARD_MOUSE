#include "usbd_def.h"
#include "usbd_hid.h"

#define CURSOR_STEP 10

#define JOY_A_KEY_Port					    GPIOC
#define JOY_A_KEY_Pin				       	GPIO_PIN_3

#define JOY_B_KEY_Port					    GPIOC
#define JOY_B_KEY_Pin					      GPIO_PIN_2

#define JOY_C_KEY_Port				    	GPIOC
#define JOY_C_KEY_Pin					      GPIO_PIN_1

#define JOY_D_KEY_Port			     		GPIOC
#define JOY_D_KEY_Pin					      GPIO_PIN_0

#define JOY_PRESS_Port							GPIOA
#define JOY_PRESS_Pin								GPIO_PIN_0

#define KEY1                        GPIOC
#define KEY1_Pin										GPIO_PIN_12

#define KEY2											  GPIOC
#define KEY2_Pin										GPIO_PIN_11

#define KEY3												GPIOC
#define KEY3_Pin										GPIO_PIN_10

#define KeyMessageSize              9
#define LEFT_CLICK                  9

#define PRESS 1
#define LOOSE 0
#define SEND_KEYBOARD 2
#define SEND_MOUSE 3
#define SEND 4
typedef enum 
{ 
  JOY_NONE = 0,
	JOY_LEFT_CLICK = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5,
	KEY_ONE = 6,
	KEY_TWO = 7,
  KEY_THREE = 8,
	KEY_ONE_TWO = 9,
	KEY_ONE_THREE = 10,
	KEY_TWO_THREE = 11
} JOYState_TypeDef
;
typedef enum
{
	KEYBOARD = 0x01,
	MOUSE = 0x10
}MODE_Type
;

typedef enum
{
	NONE = 0x00,
	A = 0x04,
	B = 0x05,
	C = 0x06,
	D = 0x07,
	E = 0x08,
	F = 0x09,
	G = 0x0A,
	H = 0x0B,
	I = 0x0C,
	J = 0x0D,
	K = 0x0E,
	L = 0x0F,
	M = 0x10,
	N = 0x11,
	O = 0x12,
	P = 0x13,
	Q = 0x14,
	R = 0x15,
	S = 0x16,
	T = 0x17,
	U = 0x18,
	V = 0x19,
	W = 0x1A,
	X = 0x1B,
	Y = 0x1C,
	Z = 0x1D,
	KEYBOARD_ONE = 0x1E,
	KEYBOARD_TWO = 0x1F,
	KEYBOARD_THREE = 0x20,
	KEYBOARD_FOUR = 0x21,
	KEYBOARD_FIVE = 0x22,
	KEYBOARD_SIX = 0x23,
	KEYBOARD_SEVEN = 0x24,
	KEYBOARD_EIGHT = 0x25,
	KEYBOARD_NINE = 0x26,
	KEYBOARD_ZERO = 0x27,
	ENTER = 0x28,
	ESCAPE = 0x29,
	BACKSPACE = 0x2A,
	TAB = 0x2B,
	SPACEBAR = 0x2C,
	MINUS = 0x2D,
	EQUAL = 0x2E,
	LEFT_BRACKET = 0x2F,
	RIGHT_BRACKET = 0x30,
	RIGHT_LINE = 0x31,
	DOT = 0x32, 		//`~
	SEMICOLON = 0x33,  //;
	QUOTATION = 0x34,  //'
	COMMA = 0x36,  //,
	FULL_STOP = 0x37,  //.
	QUESTION = 0x38,
	CAPSLOCK = 0x39,
	F_ONE = 0x3A,
	F_TWO = 0x3B,
	F_THREE = 0x3C,
	F_FOUR = 0x3D,
	F_FIVE = 0x3E,
	F_SIX = 0x3F,
	F_SEVEN = 0x40,
	F_EIGHT = 0x41,
	F_NINE = 0x42,
	F_TEN = 0x43,
	F_ELEVEN = 0x44,
	F_TWELVE = 0x45,
	PRINTSCREEN = 0x46,
	SCROOL_LOCK = 0x47,
	PAUSE = 0x48,
	INSERT = 0x49,
	HOME = 0x4A,
	PAGE_UP = 0x4B,
	DELETE = 0x4C,
	END = 0x4D,
	PAGE_DOWN = 0x4E,
	RIGHT_ARROW = 0x4F,
	LEFT_ARROW = 0x50,
	DOWN_ARROW = 0x51,
	UP_ARROW = 0x52,
	NUM_LOCK = 0x53,
}KEY_WORD
;

typedef enum
{
	LEFT_CONTROL = 0x01,
	LEFT_SHIFT = 0x02,
	LEFT_ALT = 0x04,
	LEFT_GUI = 0x08,
	RIGHT_CONTROL = 0x10,
	RIGHT_SHIFT = 0x20,
	RIGHT_ALT = 0x40,
	RIGHT_GUI = 0x80
}KEY_FUNCTION
;

/**
  *按键修改键值，WORD存放除control、alt、shift、gui以外的按键，FUNCTION存放这四个功能键，WORD和FUNCTION必须有一个为NONE
	*键值参考KEY_WORD,KEY_FUNCTION的定义
**/
static uint8_t KEY_ONE_WORD = B;
static uint8_t KEY_ONE_FUNCTION = NONE;
static uint8_t KEY_TWO_WORD = TAB;
static uint8_t KEY_TWO_FUNCTION = NONE;
static uint8_t KEY_THREE_WORD = NONE;
static uint8_t KEY_THREE_FUNCTION = LEFT_ALT;
static int tmp;

__IO JOYState_TypeDef keyDone = JOY_NONE;
extern USBD_HandleTypeDef hUsbDeviceFS;


JOYState_TypeDef Key_ReadIOPin(void)
{
	if(keyDone == JOY_NONE)
	{ 
		if(!HAL_GPIO_ReadPin(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		{
			keyDone = JOY_LEFT;
			return JOY_NONE;
		}   
		else if(!HAL_GPIO_ReadPin(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		{
			keyDone = JOY_UP;
			return JOY_NONE;
		}
		else if(!HAL_GPIO_ReadPin(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		{	
			keyDone = JOY_DOWN;
			return JOY_NONE;
		}
		else if(!HAL_GPIO_ReadPin(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		{
			keyDone = JOY_RIGHT;
			return JOY_NONE;
		}
		else if((!HAL_GPIO_ReadPin(KEY3,KEY3_Pin ))&&(!HAL_GPIO_ReadPin(KEY1,KEY1_Pin )))
		{
			keyDone = KEY_ONE_THREE;
			return JOY_NONE;
		}		
			else if((!HAL_GPIO_ReadPin(KEY2,KEY2_Pin ))&&(!HAL_GPIO_ReadPin(KEY1,KEY1_Pin )))
		{
			keyDone = KEY_ONE_TWO;
			return JOY_NONE;
		}		
		else if((!HAL_GPIO_ReadPin(KEY2,KEY2_Pin ))&&(!HAL_GPIO_ReadPin(KEY3,KEY3_Pin )))
		{
			keyDone = KEY_TWO_THREE;
			return JOY_NONE;
		}		
		else if(!HAL_GPIO_ReadPin(JOY_PRESS_Port,JOY_PRESS_Pin ))
		{
			keyDone = JOY_LEFT_CLICK;
			return JOY_NONE;
		}		
		else if(!HAL_GPIO_ReadPin(KEY1,KEY1_Pin ))
		{
			keyDone = KEY_ONE;
			return JOY_NONE;
		}		
		else if(!HAL_GPIO_ReadPin(KEY2,KEY2_Pin ))
		{
			keyDone = KEY_TWO;
			return JOY_NONE;
		}		
		else if(!HAL_GPIO_ReadPin(KEY3,KEY3_Pin ))
		{
			keyDone = KEY_THREE;
			return JOY_NONE;
		}		

	}
	else if(keyDone != JOY_NONE)
	{
		if(HAL_GPIO_ReadPin(JOY_A_KEY_Port,JOY_A_KEY_Pin) && keyDone == JOY_LEFT)
		{
			keyDone = JOY_NONE;
			return JOY_LEFT;
		}
		else if(HAL_GPIO_ReadPin(JOY_B_KEY_Port,JOY_B_KEY_Pin) && keyDone == JOY_UP)
		{
			keyDone = JOY_NONE;
			return JOY_UP;
		}
		else if(HAL_GPIO_ReadPin(JOY_C_KEY_Port,JOY_C_KEY_Pin) && keyDone == JOY_DOWN)
		{
			keyDone = JOY_NONE;
			return JOY_DOWN;
		}
		else if(HAL_GPIO_ReadPin(JOY_D_KEY_Port,JOY_D_KEY_Pin) && keyDone == JOY_RIGHT)
		{
			keyDone = JOY_NONE;
			return JOY_RIGHT;
		}
		 else if(HAL_GPIO_ReadPin(KEY3,KEY3_Pin) && keyDone == KEY_ONE_THREE && HAL_GPIO_ReadPin(KEY1,KEY1_Pin))
		{
			keyDone = JOY_NONE;
			return KEY_ONE_THREE;			
		}
		else if(HAL_GPIO_ReadPin(KEY1,KEY1_Pin) && keyDone == KEY_ONE_TWO && HAL_GPIO_ReadPin(KEY2,KEY2_Pin))
		{
			keyDone = JOY_NONE;
			return KEY_ONE_TWO;			
		}
		else if(HAL_GPIO_ReadPin(KEY2,KEY2_Pin) && keyDone == KEY_TWO_THREE && HAL_GPIO_ReadPin(KEY3,KEY3_Pin))
		{
			keyDone = JOY_NONE;
			return KEY_TWO_THREE;			
		}
		else if(HAL_GPIO_ReadPin(JOY_PRESS_Port ,JOY_PRESS_Pin) && keyDone == JOY_LEFT_CLICK)
		{
			keyDone = JOY_NONE;
			return JOY_LEFT_CLICK;			
		}
		else if(HAL_GPIO_ReadPin(KEY1,KEY1_Pin) && keyDone == KEY_ONE)
		{
			keyDone = JOY_NONE;
			return KEY_ONE;			
		}
		else if(HAL_GPIO_ReadPin(KEY2,KEY2_Pin) && keyDone == KEY_TWO)
		{
			keyDone = JOY_NONE;
			return KEY_TWO;			
		}
		else if(HAL_GPIO_ReadPin(KEY3,KEY3_Pin) && keyDone == KEY_THREE)
		{
			keyDone = JOY_NONE;
			return KEY_THREE;			
		}
	}
	return JOY_NONE;
}

JOYState_TypeDef Key_ReadIOPin_continuous(void)
{
	
	if(!HAL_GPIO_ReadPin(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		return JOY_LEFT;
	else if(!HAL_GPIO_ReadPin(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		return JOY_UP;
	else if(!HAL_GPIO_ReadPin(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		return JOY_DOWN;
	else if(!HAL_GPIO_ReadPin(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		return JOY_RIGHT;
	else if((!HAL_GPIO_ReadPin(KEY3 ,KEY3_Pin )) && (!HAL_GPIO_ReadPin(KEY1 ,KEY1_Pin )))
		return KEY_ONE_THREE;
	else if((!HAL_GPIO_ReadPin(KEY2 ,KEY2_Pin )) && (!HAL_GPIO_ReadPin(KEY1 ,KEY1_Pin )))
		return KEY_ONE_TWO;
	else if((!HAL_GPIO_ReadPin(KEY3 ,KEY3_Pin )) && (!HAL_GPIO_ReadPin(KEY2 ,KEY2_Pin )))
		return KEY_TWO_THREE;
	else if(!HAL_GPIO_ReadPin(JOY_PRESS_Port ,JOY_PRESS_Pin ))
		return JOY_LEFT_CLICK;
	else if(!HAL_GPIO_ReadPin(KEY1 ,KEY1_Pin ))
		return KEY_ONE;
	else if(!HAL_GPIO_ReadPin(KEY2 ,KEY2_Pin ))
		return KEY_TWO;
	else if(!HAL_GPIO_ReadPin(KEY3 ,KEY3_Pin ))
		return KEY_THREE;
	return JOY_NONE;
}

static uint8_t *USBD_HID_GetPos (void)
{
  int8_t  MODE=0 , x = 0,y = 0 ,z = 0 , k = 0;
  static uint8_t HID_Buffer [9]={0,0,0,0,0,0,0,0,0};
  
  switch (Key_ReadIOPin_continuous())//01键盘10鼠标
  {
  case JOY_LEFT:
		MODE = MOUSE;
		y += CURSOR_STEP;		
    break;  
  
  case JOY_RIGHT:
		MODE = MOUSE;
		y -= CURSOR_STEP;
    break;
    
  case JOY_UP:
		MODE = MOUSE;
		z += CURSOR_STEP;
    break;

  case JOY_DOWN:
		MODE = MOUSE;
		z -= CURSOR_STEP;
    break;
	
	case JOY_LEFT_CLICK:
		MODE = MOUSE;
		x = LEFT_CLICK;
		break;
		
	case KEY_ONE:
		MODE = KEYBOARD;
		x = KEY_ONE_FUNCTION;
		z = KEY_ONE_WORD;
		break;
	
	case KEY_TWO:
		MODE = KEYBOARD;
		x = KEY_TWO_FUNCTION;
		z = KEY_TWO_WORD;
		break;
	
	case KEY_THREE:
		MODE = KEYBOARD;
		x = KEY_THREE_FUNCTION;
		z = KEY_THREE_WORD;
		break;
	
	case KEY_ONE_TWO:
		MODE = KEYBOARD;
		x = KEY_ONE_FUNCTION + KEY_TWO_FUNCTION ;
		z = KEY_ONE_WORD;
		k = KEY_TWO_WORD;
		
		break;
	
	case KEY_ONE_THREE:
		MODE = KEYBOARD;
		x = KEY_ONE_FUNCTION + KEY_THREE_FUNCTION ;
		z = KEY_ONE_WORD;
		k = KEY_THREE_WORD;
		break;
	
	case KEY_TWO_THREE:
		MODE = KEYBOARD;
		x = KEY_TWO_FUNCTION + KEY_THREE_FUNCTION ;
		z = KEY_TWO_WORD ;
		k = KEY_THREE_WORD ;
		break;
		
	case JOY_NONE:
		break;
	
  }
	
	HID_Buffer[0] = MODE;
  HID_Buffer[1] = x;
  HID_Buffer[2] = y;
  HID_Buffer[3] = z;
  HID_Buffer[4] = k;
  
  return HID_Buffer;
}

void Control(void)
{
  
  uint8_t *buf;

  buf = USBD_HID_GetPos();
  if((buf[1] !=0) ||(buf[2] != 0) ||(buf[3] != 0) ||(buf[4] != 0) )
  {
    USBD_HID_SendReport (&hUsbDeviceFS, 
                         buf,
                         KeyMessageSize);
		tmp = SEND;
  } 
	else tmp = LOOSE;
}

int getValue(void)
{
	return tmp;
}

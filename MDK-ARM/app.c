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

extern uint8_t  GetInfraredKey(void);

typedef enum  //开发板按键引脚定义
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
typedef enum  //键盘模式定义
{
	KEYBOARD = 0x01,
	MOUSE = 0x10
}MODE_Type
;

typedef enum  //键盘键值定义
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

typedef enum  //键盘功能键键值定义
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

typedef struct T9_Infrared  //红外遥控所需定义
{
  uint32_t  KEY_VALUE;   
	uint16_t  TriTime[2];  
	uint8_t   DataBit;
	uint8_t   FrameStart;
  uint8_t   KEY_Count;	
	uint8_t   TriPolarity;
	
}T9_InfraredDev;

typedef enum  //遥控器键值定义（不同遥控器做不同适配）
{
	Remote_Control_ONE						 = 	0x00,
	Remote_Control_TWO 						 = 	0x10,
	Remote_Control_THREE 					 = 	0x11,
	Remote_Control_FOUR 					 = 	0x13,
	Remote_Control_FIVE						 = 	0x14,
	Remote_Control_SIX 						 = 	0x15,
	Remote_Control_SEVEN 					 = 	0x17,
	Remote_Control_EIGHT 					 = 	0x18,
	Remote_Control_NINE 					 = 	0x19,
	Remote_Control_ZERO 					 = 	0x1B,
	Remote_Control_PAGE_UP 				 = 	0x42,
	Remote_Control_PAGE_DOWN 			 = 	0x0F,
	Remote_Control_F_ONE 					 = 	0x45,
	Remote_Control_F_TWO 					 =	0x12,
	Remote_Control_F_THREE 				 =	0x51,
	Remote_Control_F_FOUR 				 =	0x5B,
	Remote_Control_F_FIVE 				 =	0x44,
	Remote_Control_F_SIX 					 =	0x50,
	Remote_Control_F_SEVEN 				 =	0x43,
	Remote_Control_F_EIGHT				 =	0x1A,
	Remote_Control_F_NINE 				 =	0x04,
	Remote_Control_F_TEN 					 =	0x59,
	Remote_Control_F_ELEVEN				 =	0x57,
	Remote_Control_F_TWELVE				 =	0x08,
	Remote_Control_ARROW_UP 			 =	0x47,
	Remote_Control_ARROW_DOWN			 =	0x4D,
	Remote_Control_ARROW_LEFT 		 =	0x49,
	Remote_Control_ARROW_RIGHT		 =	0x4B,
	Remote_Control_ENTER					 =	0x4A,
	Remote_Control_BACKSPACE			 =	0x40,
	Remote_Control_SPACEBAR				 =	0x46,
	Remote_Control_LEFT_GUI				 =	0x0B,
	Remote_Control_ALT_PLUS_TAB	   =	0x1D,
	Remote_Control_ALT_PLUS_F_FOUR =	0x1F 
}Remote_Control_KeyNum;
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
extern TIM_HandleTypeDef htim9;

JOYState_TypeDef Key_ReadIOPin(void)
{
	if(!HAL_GPIO_ReadPin( JOY_C_KEY_Port,JOY_C_KEY_Pin ))
		return JOY_LEFT;
	else if(!HAL_GPIO_ReadPin( JOY_A_KEY_Port,JOY_A_KEY_Pin ))
		return JOY_UP;
	else if(!HAL_GPIO_ReadPin( JOY_D_KEY_Port,JOY_D_KEY_Pin ))
		return JOY_DOWN;
	else if(!HAL_GPIO_ReadPin( JOY_B_KEY_Port,JOY_B_KEY_Pin ))
		return JOY_RIGHT;
	else if((!HAL_GPIO_ReadPin( KEY3 ,KEY3_Pin )) && (!HAL_GPIO_ReadPin(KEY1 ,KEY1_Pin )))
		return KEY_ONE_THREE;
	else if((!HAL_GPIO_ReadPin( KEY2 ,KEY2_Pin )) && (!HAL_GPIO_ReadPin(KEY1 ,KEY1_Pin )))
		return KEY_ONE_TWO;
	else if((!HAL_GPIO_ReadPin( KEY3 ,KEY3_Pin )) && (!HAL_GPIO_ReadPin(KEY2 ,KEY2_Pin )))
		return KEY_TWO_THREE;
	else if(!HAL_GPIO_ReadPin( JOY_PRESS_Port ,JOY_PRESS_Pin ))
		return JOY_LEFT_CLICK;
	else if(!HAL_GPIO_ReadPin( KEY1 ,KEY1_Pin ))
		return KEY_ONE;
	else if(!HAL_GPIO_ReadPin( KEY2 ,KEY2_Pin ))
		return KEY_TWO;
	else if(!HAL_GPIO_ReadPin( KEY3 ,KEY3_Pin ))
		return KEY_THREE;
	return JOY_NONE;
}

static uint8_t *USBD_HID_GetPos (void)
{
  int8_t  MODE=0 , x = 0,y = 0 ,z = 0 , k = 0;
  static uint8_t HID_Buffer [9]={0x01,0,0,0,0,0,0,0,0};
  
  switch (Key_ReadIOPin())//01键盘10鼠标
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
		MODE = KEYBOARD;
		break;
	
	default:	
		MODE = KEYBOARD;
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
    USBD_HID_SendReport (&hUsbDeviceFS , buf , KeyMessageSize);
		tmp = SEND;
  } 
	else tmp = LOOSE;
}
static uint8_t *Package(uint32_t a)  //红外遥控打包
{
	int8_t  MODE=0 , x = 0,y = 0 ,z = 0 , k = 0;
	static uint8_t Hid_Buffer [9]={0,0,0,0,0,0,0,0,0};
	switch(a)
	{
		case Remote_Control_ONE : 
							 z = KEYBOARD_ONE;
							 MODE = KEYBOARD;
						   break;
		case Remote_Control_TWO : 
							 z = KEYBOARD_TWO;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_THREE : 
							 z = KEYBOARD_THREE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_FOUR : 
							 z = KEYBOARD_FOUR;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_FIVE : 
							 z = KEYBOARD_FIVE;
							 MODE = KEYBOARD;
							break;		
		case Remote_Control_SIX :
							 z = KEYBOARD_SIX;
							 MODE = KEYBOARD;
							 break;		
		case Remote_Control_SEVEN : 
							 z = KEYBOARD_SEVEN;
							 MODE = KEYBOARD;
						 	 break;		
		case Remote_Control_EIGHT : 
							 z = KEYBOARD_EIGHT;
							 MODE = KEYBOARD;	
							 break;		
		case Remote_Control_NINE : 
							 z = KEYBOARD_NINE;
							 MODE = KEYBOARD;
							 break;	
		case Remote_Control_ZERO : 
							 z = KEYBOARD_ZERO;
							 MODE = KEYBOARD;
							 break;		
		case Remote_Control_PAGE_UP : 
							 z = PAGE_UP;
							 MODE = KEYBOARD;
							 break;		
		case Remote_Control_PAGE_DOWN : 
							 z = PAGE_DOWN;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_ONE : 
							 z = F_ONE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_TWO : 
							 z = F_TWO;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_THREE : 
						   z = F_THREE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_FOUR : 
							 z = F_FOUR;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_FIVE: 	
							 z = F_FIVE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_SIX : 
							 z = F_SIX;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_SEVEN : 
							 z = F_SEVEN;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_EIGHT : 
							 z = F_EIGHT;
							 MODE = KEYBOARD;
							 break;					
		case Remote_Control_F_NINE : 
							 z = F_NINE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_TEN : 
							 z = F_TEN;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_ELEVEN :
							 z = F_ELEVEN;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_F_TWELVE : 
							 z = F_TWELVE;
							 MODE = KEYBOARD;
							 break;
		case Remote_Control_ARROW_UP ://right
							z = UP_ARROW ;	
							MODE = KEYBOARD;
              break;
		case Remote_Control_ARROW_DOWN :	//right
							z = DOWN_ARROW;
						  MODE = KEYBOARD;
							break;					
		case Remote_Control_ARROW_LEFT : //right
							z = LEFT_ARROW;
							MODE = KEYBOARD;
							break;
		case Remote_Control_ARROW_RIGHT : //right
							z = RIGHT_ARROW;
							MODE = KEYBOARD;
							break;
		case Remote_Control_ENTER : 
							z = ENTER;
							MODE = KEYBOARD;
							break;
		case Remote_Control_BACKSPACE : 
							z = BACKSPACE;
						  MODE = KEYBOARD;
							break;
		case Remote_Control_SPACEBAR : 
							z = SPACEBAR;
							MODE = KEYBOARD;
							break;
		case Remote_Control_LEFT_GUI : 
							x = LEFT_GUI;
							MODE = KEYBOARD;
							break;
		case Remote_Control_ALT_PLUS_TAB : 
							x = LEFT_ALT;
							z = TAB;
							MODE = KEYBOARD;
							break;
		case Remote_Control_ALT_PLUS_F_FOUR : 
							x = LEFT_ALT;
							z = F_FOUR;
							MODE = KEYBOARD;
							break;
		default: break;
	}
	Hid_Buffer[0] = MODE;
  Hid_Buffer[1] = x;
  Hid_Buffer[2] = y;
  Hid_Buffer[3] = z;
  Hid_Buffer[4] = k;
  
  return Hid_Buffer;
}

void InfraredControl(void)
{
			uint8_t *Buf ;
			Buf = Package(GetInfraredKey());  //键值打包
			USBD_HID_SendReport (&hUsbDeviceFS, Buf , 9);	
}



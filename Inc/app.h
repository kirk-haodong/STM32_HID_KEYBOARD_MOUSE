#include "usbd_def.h"
#include "usbd_hid.h"

#define CURSOR_STEP 5

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

__IO JOYState_TypeDef keydone = JOY_NONE;
extern USBD_HandleTypeDef hUsbDeviceFS;
JOYState_TypeDef Key_ReadIOPin(void);
static uint8_t *USBD_HID_GetPos (void);
void Control(void);
void keyboardControl(void);
uint8_t keyScan(void);
void InfraredControl(void);



//KEYBOARD_TypeDef KeyBOARD_ReadIOPin(void);
//KEYBOARD_TypeDef Keyboard_ReadIOPin_continuous(void);
//static uint8_t *USBD_HID_KEYBOARD_GetPos (void);
//void keyboardControl(void);

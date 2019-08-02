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
  JOY_UP = 5
} JOYState_TypeDef
;
__IO JOYState_TypeDef keydone = JOY_NONE;
extern USBD_HandleTypeDef hUsbDeviceFS;
JOYState_TypeDef Key_ReadIOPin(void);
JOYState_TypeDef Key_ReadIOPin_continuous(void);
static uint8_t *USBD_HID_GetPos (void);
void mouseControl(void);

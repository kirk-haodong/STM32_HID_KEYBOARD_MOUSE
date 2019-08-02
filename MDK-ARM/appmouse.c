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

#define KEY1                        GPIOC
#define KEY1_Pin										GPIO_PIN_12

#define KEY2											  GPIOC
#define KEY2_Pin										GPIO_PIN_11

typedef enum 
{ 
  JOY_NONE = 0,
	JOY_LEFT_CLICK = 1,
	JOY_RIGHT_CLICK = 6,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef
;
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
		else if(!HAL_GPIO_ReadPin(KEY1,KEY1_Pin))
		{
			keyDone = JOY_LEFT_CLICK;
			return JOY_NONE;
		}
		else if(!HAL_GPIO_ReadPin(KEY2,KEY2_Pin))
		{
			keyDone = JOY_RIGHT_CLICK;
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
		else if(HAL_GPIO_ReadPin(KEY1,KEY1_Pin) && keyDone == JOY_LEFT_CLICK)
		{
			keyDone = JOY_NONE;
			return JOY_LEFT_CLICK;
		}
		else if(HAL_GPIO_ReadPin(KEY2,KEY2_Pin) && keyDone == JOY_RIGHT_CLICK)
		{
			keyDone = JOY_NONE;
			return JOY_RIGHT_CLICK;
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
	else if(!HAL_GPIO_ReadPin(KEY1,KEY1_Pin))
		return JOY_LEFT_CLICK;
	else if(!HAL_GPIO_ReadPin(KEY2,KEY2_Pin))
		return JOY_RIGHT_CLICK;
	return JOY_NONE;
}
static uint8_t *USBD_HID_GetPos (void)
{
  int8_t  x = 0 , y = 0 ,z = 0;
  static uint8_t HID_Buffer [4]={0,0,0,0};
  
  switch (Key_ReadIOPin_continuous())
  {
  case JOY_LEFT:
		x += CURSOR_STEP;		
    break;  
  
  case JOY_RIGHT:
		x -= CURSOR_STEP;
    break;
    
  case JOY_UP:
		y += CURSOR_STEP;
    break;

  case JOY_DOWN:
		y -= CURSOR_STEP;
    break;
	
	case JOY_LEFT_CLICK:
		z = 9;
		break;
	
	case JOY_RIGHT_CLICK:
		z = 0x0a;
		break;
		
	case JOY_NONE:
		break;
	
  }

  HID_Buffer[0] = z;
  HID_Buffer[1] = x;
  HID_Buffer[2] = y;
  HID_Buffer[3] = 0;
  
  return HID_Buffer;
}
void mouseControl(void)
{
  
  uint8_t *buf;
	uint8_t initbuf[4]={0,0,0,0};
  
  buf = USBD_HID_GetPos();
  if((buf[0] !=0) ||(buf[1] != 0) ||(buf[2] != 0))
  {
    USBD_HID_SendReport (&hUsbDeviceFS, 
                         buf,
                         4);
		USBD_HID_SendReport (&hUsbDeviceFS, 
                         initbuf,
                         4);
  } 
}

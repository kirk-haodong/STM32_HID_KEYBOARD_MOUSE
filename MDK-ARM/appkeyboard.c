#include "stdint.h"
#include "usbd_def.h"
#include "usbd_hid.h"
#define KeyMessageSize 8

extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t KeySendBuff[8]={0,0,0,0,0,0,0,0};
uint8_t KeySendBuffInit[8]={0,0,0,0,0,0,0,0};
void keyboardControl(void);
uint8_t keyScan(void);

uint8_t keyScan(void)
{

	uint8_t key=0x01;
	if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_12)){
			HAL_Delay(100);
		if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_12))
		KeySendBuff[3]=0x04;
		key=0x00;
		}
	if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_11)){
		HAL_Delay(100);
		if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_11))
			KeySendBuff[0]=0x01;
		key=0x00;
	}
	if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_10)){
	HAL_Delay(100);
		if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_10)&&!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_11))
			KeySendBuff[0]=0x03;
		else if(!HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_10))
			KeySendBuff[0]=0x02;
		key=0x00;
	}
	return key;
}

void keyboardControl(void)
{
	USBD_HID_SendReport(&hUsbDeviceFS,KeySendBuff,KeyMessageSize);
	HAL_Delay (100);
	USBD_HID_SendReport(&hUsbDeviceFS,KeySendBuffInit,KeyMessageSize);
	KeySendBuff[0]=0;
	KeySendBuff[3]=0;
}

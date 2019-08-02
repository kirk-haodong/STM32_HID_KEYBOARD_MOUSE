#include "stdint.h"
#include "stm32f2xx_hal_gpio.h"
#include "usbd_def.h"
#include "usbd_hid.h"
#define KeyMessageSize 8

extern USBD_HandleTypeDef hUsbDeviceFS;
void keyboardControl(void);
uint8_t keyScan(void);

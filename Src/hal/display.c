/*
 * display.c
 *
 *  Created on: 2017/08/26
 *      Author: take-iwiw
 */
#include <stdio.h>
#include "cmsis_os.h"
#include "main.h"
#include "common.h"
#include "stm32f4xx_hal.h"
#include "../driver/lcdIli9341/lcdIli9341.h"
#include "display.h"

/*** Internal Const Values, Macros ***/

/*** Internal Static Variables ***/

/*** Internal Function Declarations ***/
static RET display_checkArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

/*** External Function Defines ***/
RET display_init()
{
  return lcdIli9341_init();
}

RET display_setArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
  if(display_checkArea(xStart, yStart, xEnd, yEnd) == RET_OK){
    lcdIli9341_setArea(xStart, yStart, xEnd, yEnd);
    return RET_OK;
  }
  return RET_ERR_PARAM;
}

RET display_setAreaRead(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
  if(display_checkArea(xStart, yStart, xEnd, yEnd) == RET_OK){
    lcdIli9341_setAreaRead(xStart, yStart, xEnd, yEnd);
    return RET_OK;
  }
  return RET_ERR_PARAM;
}

RET display_drawRect(uint16_t xStart, uint16_t yStart, uint16_t width, uint16_t height, uint16_t color)
{
  if(display_checkArea(xStart, yStart, xStart + width - 1, yStart + height - 1) == RET_OK){
    lcdIli9341_drawRect(xStart, yStart, width, height, color);
    return RET_OK;
  }
  return RET_ERR_PARAM;

}

void* display_getDisplayHandle()
{
  (void*)lcdIli9341_getDrawAddress();
}

uint32_t display_getPixelFormat()
{
  return DISPLAY_PIXEL_FORMAT_RGB565;
}


void display_drawBuffer(void* canvasHandle, uint32_t pixelNum)
{
  uint16_t *pBuff = canvasHandle;
  for(uint32_t x = 0; x < pixelNum; x++) {
    *((uint16_t*)lcdIli9341_getDrawAddress()) = *pBuff;
    pBuff++;
  }
}

inline void display_putPixelRGB565(uint16_t rgb565)
{
  *((uint16_t*)lcdIli9341_getDrawAddress()) = rgb565;
}


void display_readLineRGB888(uint8_t *p_buff, uint32_t width)
{
  /* can I use DMA for this? */
  volatile uint16_t* p_lcdAddr = (volatile uint16_t* )(lcdIli9341_getDrawAddress());
  for(uint32_t x = 0; x < width / 2; x++){
    uint16_t data0 = *p_lcdAddr;
    uint16_t data1 = *p_lcdAddr;
    uint16_t data2 = *p_lcdAddr;
    p_buff[x*6 + 0] = data0 >> 8;
    p_buff[x*6 + 1] = data0 & 0x00FF;
    p_buff[x*6 + 2] = data1 >> 8;
    p_buff[x*6 + 3] = data1 & 0x00FF;
    p_buff[x*6 + 4] = data2 >> 8;
    p_buff[x*6 + 5] = data2 & 0x00FF;
  }
}


/*** Internal Function Defines ***/
static RET display_checkArea(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
  if( (xEnd < LCD_ILI9342_WIDTH) && (yEnd < LCD_ILI9342_HEIGHT) ){
    return RET_OK;
  }
  return RET_ERR_PARAM;
}


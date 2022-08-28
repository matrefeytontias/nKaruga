#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TIMER 2

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out);
void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect *out);
int sq(int);
int cube(int);
int interpolatePathFloat(int, float[], float[], int[], int, Rect*);

void initBuffering();
void constrainFrameRate(int);
void displayFrameRate();
void updateScreen();
void updateKeys();
void deinitBuffering();
void timer_load(unsigned, Uint32);
unsigned timer_read(unsigned);
void clearBufferB();
void clearBufferW();
void clearBuffer(unsigned short);
unsigned short getPixelUnsafe(const unsigned short*, unsigned int, unsigned int);
unsigned short getPixel(const unsigned short*, unsigned int, unsigned int);
void setPixelUnsafe(unsigned int, unsigned int, unsigned short);
void setPixel(unsigned int, unsigned int, unsigned short);
void setPixelRGB(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
void drawHLine(int, int, int, unsigned short);
void drawVLine(int, int, int, unsigned short);
void fillRect(int, int, int, int, unsigned short);
void drawSprite(const unsigned short*, int, int, int, unsigned short);
void drawSpritePart(const unsigned short*, int, int, const Rect*, int, unsigned short);
void drawSpriteScaled(const unsigned short*, const Rect*, int, unsigned short);
void drawSpriteRotated(const unsigned short*, const Rect*, const Rect*, Fixed, int, unsigned short);
void drawLine(int, int, int, int, unsigned short);
void drawPolygon(unsigned short, int, ...);
void fillCircle(int, int, int, unsigned short);
void fillEllipse(int, int, int, int, unsigned short);
void drawString(int*, int*, int, const char*, unsigned short, unsigned short);
void drawDecimal(int*, int*, int, unsigned short, unsigned short);
void drawChar(int*, int*, int, char, unsigned short, unsigned short);
void drawStringF(int*, int*, int, unsigned short, unsigned short, const char*, ...);
int numberWidth(int);
int stringWidth(const char*);
void wait_no_key_pressed(t_key);
int get_key_pressed(t_key*);
int isKey(t_key, t_key);

unsigned short * loadBMP(const char*, unsigned short);

#define BUFF_BYTES_SIZE (320*240*2)
unsigned short *BUFF_BASE_ADDRESS;
SDL_Texture *MAIN_SCREEN;

#ifdef __cplusplus
}
#endif
#pragma once

#include "raylib.h"

#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <time.h>


#define SCREEN_W 960
#define SCREEN_H 720

#define SCALE 2

#define GRID_W (SCREEN_W / SCALE)
#define GRID_H (SCREEN_H / SCALE)

#define MIN_BRUSH 1
#define MAX_BRUSH (75 * SCALE)

extern int tick_count;

bool RandChance(float chance);
bool RandRoll(int roll);
int RandRange(int min, int max);
void DrawTextShadow(const char* text, int x, int y, int shadow_offset_x, int shadow_offset_y, int font_size, Color text_color, Color shadow_color);
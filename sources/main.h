#pragma once

#include "raylib.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <string>
#include <time.h>


// Screen Parameters
//
#define SCREEN_W 960
#define SCREEN_H 720

#define SCALE 2


// Brush Parameters
//
#define MIN_BRUSH 1
#define MAX_BRUSH (200 / SCALE)

extern int tick_count;

bool RandRoll(int roll);
bool RandChance(float chance);
int RandRange(int min, int max);
Color OffsetColor(Color color, int min, int max);


// Vector2i
//
struct Vector2i {
	int x = 0;
	int y = 0;
};

constexpr Vector2i V_NONE = {0, 0};
constexpr Vector2i V_LEFT = {-1, 0};
constexpr Vector2i V_RIGHT = {1, 0};
constexpr Vector2i V_BELOW = {0, 1};
constexpr Vector2i V_BELOW_LEFT = {-1, 1};
constexpr Vector2i V_BELOW_RIGHT = {1, 1};
constexpr Vector2i V_ABOVE = {0, -1};
constexpr Vector2i V_ABOVE_LEFT = {-1, -1};
constexpr Vector2i V_ABOVE_RIGHT = {1, -1};


// Element IDs
//

constexpr unsigned int AIR = 0;
constexpr unsigned int SAND = 1;
constexpr unsigned int WATER = 2;
constexpr unsigned int WOOD = 3;
constexpr unsigned int FIRE = 4;
constexpr unsigned int SALT = 5;
constexpr unsigned int CONCRETE_POWDER = 6;
constexpr unsigned int CONCRETE = 7;
constexpr unsigned int OIL = 8;
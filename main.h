// main.h

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utility.h"

// Window Parameters
#define SCREEN_W 1280
#define SCREEN_H 768

// Game Parameters
#define SCALE_FACTOR 2
#define GRID_WIDTH (SCREEN_W / SCALE_FACTOR)
#define GRID_HEIGHT (SCREEN_H / SCALE_FACTOR)

#define MIN_BRUSH_SIZE 4
#define MAX_BRUSH_SIZE 100


// Globals
int tick_count = 0;
int particle_count = 0;

int brush_material = 1;
int brush_density = 4;
int brush_size = 10;

bool world_borders = true;
bool spouts_enabled = false;


// Galaxian Colors
#define G_BLACK        (Color) { 25, 25, 34, 255 }        // BlueBlack
#define G_WHITE        (Color) { 241, 241, 241, 255 }     // Dark White
#define G_GRAY         (Color) { 138, 152, 162, 255 }     // Unofficial Gray
#define G_DARKGRAY     (Color) { 69, 77, 82, 255 }        // Unofficial Gray But 50% Darker
#define G_RED          (Color) { 239, 71, 111, 255 }      // Technically Pink
#define G_ORANGE       (Color) { 255, 117, 56, 255 }      // Just Orange
#define G_YELLOW       (Color) { 255, 206, 92, 255 }      // Off-Yellow
#define G_GREEN        (Color) { 7, 197, 102, 255 }       // Pokemon Emerald
#define G_TEAL         (Color) { 4, 174, 173, 255 }       // Not Really Teal
#define G_BLUE         (Color) { 1, 151, 244, 255 }       // Blue, His House
#define G_PURPLE       (Color) { 159, 89, 197, 255 }      // "Grape" Purple

// More Includes
#include "particle.c"
#include "update.c"

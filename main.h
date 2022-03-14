#include "raylib.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"


// ------------------------------
// Macros
// ------------------------------
#define SCREEN_W 800
#define SCREEN_H 600

#define MIN_BRUSH 4 // Anything lower than 4 doesn't work, I'll figure that out later
#define MAX_BRUSH 100

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

//
// Materials
//
#define P_EMPTY 		0
#define P_SAND 			1
#define P_WATER 		2
#define P_STONE 		3
#define P_SALT 			4
#define P_OIL 			5
#define P_FIRE 			6
#define P_SMOKE 		7
#define P_STEAM 		8
#define P_NANOBOTS 	9
#define P_PLANT 		10


// ------------------------------
// Custom Colors
// ------------------------------
#define G_BLACK			(Color) { 25, 25, 34, 255 }			// BlueBlack
#define G_WHITE			(Color) { 241, 241, 241, 255 }	// Dark White
#define G_GRAY			(Color) { 138, 152, 162, 255 } 			// Unofficial Gray
#define G_DARKGRAY 	(Color) { 69, 77, 82, 255 }					// Unofficial Gray But 50% Darker
#define G_RED 			(Color) { 239, 71, 111, 255 }		// Technically Pink
#define G_ORANGE 		(Color) { 255, 117, 56, 255 }		// Just Orange
#define G_YELLOW 		(Color) { 255, 206, 92, 255 }  	// Off-Yellow
#define G_GREEN 		(Color) { 7, 197, 102, 255 }		// Pokemon Emerald
#define G_TEAL 			(Color) { 4, 174, 173, 255 }		// Not Really Teal
#define G_BLUE 			(Color) { 1, 151, 244, 255 }		// Blue, His House
#define G_PURPLE 		(Color) { 159, 89, 197, 255 }		// "Grape" Purple


// ------------------------------
// Globals
// ------------------------------
int tick_count = 0;
int particle_count = 0;

int brush_material = 1; // Default is Sand
int brush_density = 2; // Lower number is more dense
int brush_size = 10; // Radius of brush circle

bool world_borders = true; // Constrain particles to window


// ------------------------------
// Base Particle
// ------------------------------
typedef struct Particle {
	unsigned int id; // Material ID
	int fall; // Fall speed
	int flow; // Flow amount
	int super; // Paint priority
	Color color; // Color
	bool falling; // If particle is falling
} Particle;


// ------------------------------
// Particle Materials
// ------------------------------
const Particle materials[] = {
	[0] = { // Empty
		.id = 0,
		.fall = 0,
		.flow = 0,
		.super = 1,
		.color = G_BLACK,
	},

	[1] = { // Sand
		.id = 1,
		.fall = 2,
		.flow = 2,
		.super = 4,
		.color = G_YELLOW,
	},

	[2] = { // Water
		.id = 2,
		.fall = 2,
		.flow = 6,
		.super = 5,
		.color = G_BLUE,
	},

	[3] = { // Stone
		.id = 3,
		.fall = 0,
		.flow = 0,
		.super = 3,
		.color = G_GRAY,
	},

	[4] = { // Salt
		.id = 4,
		.fall = 2,
		.flow = 1,
		.super = 4,
		.color = G_WHITE,
	},

	[5] = { // Oil
		.id = 5,
		.fall = 2,
		.flow = 5,
		.super = 5,
		.color = G_RED,
	},

	[6] = { // Fire
		.id = 6,
		.fall = -2,
		.flow = 2,
		.super = 0,
		.color = G_ORANGE,
	},

	[7] = { // Smoke
		.id = 7,
		.fall = -3,
		.flow = 4,
		.super = 6,
		.color = G_DARKGRAY,
	},

	[8] = { // Steam
		.id = 8,
		.fall = -3,
		.flow = 4,
		.super = 6,
		.color = G_WHITE,
	},

	[9] = { // Nanobots
		.id = 9,
		.fall = 0,
		.flow = 0,
		.super = 6,
		.color = G_PURPLE,
	},

	[10] = { // Plant
		.id = 11,
		.fall = 0,
		.flow = 0,
		.super = 3,
		.color = G_GREEN,
	}
};

const char *material_names[] = {
	[0] = "Empty",
	[1] = "Sand",
	[2] = "Water",
	[3] = "Stone",
	[4] = "Salt",
	[5] = "Oil",
	[6] = "Fire",
	[7] = "Smoke",
	[8] = "Steam",
	[9] = "Nanobots",
	[10] = "Plant",
};


// ------------------------------
// Particle Functions
// ------------------------------

// Return if grid position is empty
bool IsEmpty(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	return (grid[x][y].id == 0);
}

// Return if grid position is in bounds
bool InBounds(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	return x > 0 && x < SCREEN_W && y > 0 && y < SCREEN_H;
}

// Return if particle at position is falling
bool IsFalling(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	return (grid[x][y].falling);
}

// Return if particle 1 is superior to particle 2
bool IsSuper(int p1, int p2) {
	if (materials[p1].super == 0 || materials[p2].super == 0) { return true; }
	return (materials[p1].super <= materials[p2].super);
}

// Update Particle Count
void UpdateParticleCount(Particle grid[SCREEN_W][SCREEN_H]) {
	int n = 0;

	for (int y = 0; y < SCREEN_H; y++) {
		for (int x = 0; x < SCREEN_W; x++) {
			if (grid[x][y].id > 0) {
				n += 1;
			}
		}
	}

	particle_count = n;
}

// Generate new particle struct
Particle NewParticle(int id) {
	Particle np;

	np.id = materials[id].id;
	np.fall = materials[id].fall;
	np.flow = materials[id].flow;
	np.super = materials[id].super;
	np.color = materials[id].color;

	np.falling = true;

	return np;
}

// Get particle ID at position
Particle GetParticle(Particle grid[SCREEN_W][SCREEN_H], int x, int y) {
	return (grid[x][y]);
}

// Set grid position to specified particle
void SetParticle(Particle grid[SCREEN_W][SCREEN_H], int id, int x, int y) {
	Particle new_particle = NewParticle(id);
	grid[x][y] = new_particle;
}

// Paint particles to the grid with a specified brush size
void PaintParticles(Particle grid[SCREEN_W][SCREEN_H], int brushSize, int brushDensity, int id, int x, int y) {
	int n = 0; // Number of particles painted
	int tries = 0; // Number of attempts to paint a particle
	int max_tries = 10; // Number of attempts before moving on
	int goal = brushSize / brushDensity; // Goal amount of particles to paint

	while (n < goal) {
		float r = brushSize * sqrt((float)rand() / RAND_MAX);
		float t = (float)rand() / RAND_MAX * 2 * M_PI; // Theta

		int pX = round(x + r * cos(t)); 
		int pY = round(y + r * sin(t));

		if (InBounds(grid, pX, pY) && IsEmpty(grid, pX, pY)) {
			SetParticle(grid, id, pX, pY);
			n += 1; // Increment number painted
			tries = 0; // Reset attempts on success
		} else {
			if (tries >= max_tries) {
				n += 1; // Skip to next pixel attempt
				tries = 0; // Reset attempts
			} else {
				tries += 1; // Increment attempts
			}
		}
	}
}


// Paint solid particles (fill entire brush circle)
void PaintSolidParticles(Particle grid[SCREEN_W][SCREEN_H], int brushSize, int id, int x, int y) {
	for (int i = y-brushSize; i < y+brushSize; i++) {
		for (int j = x-brushSize; j < x+brushSize; j++) {
			if (InBounds(grid, j, i)) {
				SetParticle(grid, id, j, i);
			}
		}
	}
}


//
// Main Update + Draw Functions
//

// Update all particles in the grid
#include "update.h"

void UpdateParticles(Particle grid[SCREEN_W][SCREEN_H]) {
	bool is_even_tick = ((tick_count % 2) == 0);
	int dir = is_even_tick ? 0 : 1;

	for (int y = SCREEN_H; y > 0; y--) {
		for (int x = dir ? 0 : SCREEN_W - 1; dir ? x < SCREEN_W : x > 0; dir ? x++ : x--) {
			if (!IsEmpty(grid, x, y)) {
				Particle p = grid[x][y];

				switch(p.id) {
					case 1: // Sand
						UpdateSand(grid, x, y);
						break;
					case 2: // Water
						UpdateWater(grid, x, y);
						break;
					case 3: // Stone
						break;
				}
			}
		}
	}
}

// Draw all particles in the grid
void DrawParticles(Particle grid[SCREEN_W][SCREEN_H]) {
	for (int y = SCREEN_H; y > 0; y--) {
		for (int x = 0; x < SCREEN_W; x++) {
			Particle p = grid[x][y];
			if (p.id > 0) {
				DrawPixel(x, y, p.color);
			}
		}
	}
}


// ------------------------------
// Utility Functions
// ------------------------------
// Draw text with font (easier than DrawTextEx)
void DrawTextF(Font font, const char *text, int x, int y, float size, Color color) {
	Vector2 pos = {x, y};
	DrawTextEx(font, text, pos, size, 1, color);
}

// Prepend character to the beginning of a string
void Prepend(char* str, const char* ch) {
	size_t len = strlen(str);
	memmove(str + len, str, strlen(str) + 1);
	memcpy(str, ch, len);
}

char FormatNumber(char *number) {
	char *fNumber;
	int size = strlen(number);
	int count = 0;
	
	for (int i = size; i > 0; i--) {
		Prepend(fNumber, &number[i]);
		count += 1;

		if (count == 3) {
			Prepend(fNumber, ",");
			Prepend(fNumber, &number[i]);
			count = 0;
		}
	}

	return *fNumber;
}

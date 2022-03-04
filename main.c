#include "main.h"
#include <time.h>

// Known Bugs
// ------------------------------
// 1. Painting particles in an area with no empty spaces gets stuck in the PaintParticles while loop
// 		- Fix: Add limit to number of attempts to paint a particle before moving to the next
//
// 2. Program occasionally hangs/crashes when painting near the edges of the window
// 		- Possible due to bug #1, also possibly due to bad memory access
//
// 3. Some particles are being placed wrong during updates, causing the number of particles to go down when it shouldn't
// 		- This has happened a few times, it's a mystery what's causing it this time
// 		- Most noticeable with water but it happens with sand also


// ------------------------------
// Application
// ------------------------------
int main(void) {
	InitWindow(SCREEN_W, SCREEN_H, "SandSim");
	SetTargetFPS(600);
	srand(time(NULL));

	Font FONT_UI = LoadFont("RandomEncounter.ttf");
	
	Vector2 mouse_pos;
	HideCursor();

	//Particle grid[SCREEN_W][SCREEN_H];
	//Particle* grid = malloc((SCREEN_W * SCREEN_H) * sizeof(Particle));
	Particle(*grid)[] = malloc((SCREEN_W * SCREEN_H) * sizeof(Particle));

	for (int y = 0; y < SCREEN_H; y++) {
		for (int x = 0; x < SCREEN_W; x++) {
			SetParticle(grid, 0, x, y);
		}
	}

	//
	// Main Game Loop
	//
	while (!WindowShouldClose()) {
		//
		// Update
		//
		
		// Mouse input
		mouse_pos = GetMousePosition();

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (brush_material == 0 || brush_material == 3) {
				PaintSolidParticles(grid, brush_size, brush_material, mouse_pos.x, mouse_pos.y);
			} else {
				PaintParticles(grid, brush_size, brush_density, brush_material, mouse_pos.x, mouse_pos.y);
			}
		}

		if (GetMouseWheelMove()) {
			brush_size -= 1 * GetMouseWheelMove();
			if (brush_size > MAX_BRUSH) { brush_size = MAX_BRUSH; }
			if (brush_size < MIN_BRUSH) { brush_size = MIN_BRUSH; }
		}

		// Keyboard input
		if (IsKeyPressed(KEY_ZERO)) 	{ brush_material = 0; } // Empty
		if (IsKeyPressed(KEY_ONE)) 		{ brush_material = 1; } // Sand
		if (IsKeyPressed(KEY_TWO)) 		{ brush_material = 2; } // Water
		if (IsKeyPressed(KEY_THREE))	{ brush_material = 3; } // Stone

		if (IsKeyPressed(KEY_SEVEN))	{ brush_material = 7; } // Smoke

		// Spouts
		// PaintParticles(grid, 5, 4, 1, 200, 0);
		// PaintParticles(grid, 5, 4, 2, 600, 0);

		// Update particles
		UpdateParticles(grid);
		UpdateParticleCount(grid);

		// Other
		tick_count += 1;

		//
		// Draw
		//
		BeginDrawing();
			ClearBackground(G_BLACK);

			// Draw all particles
			DrawParticles(grid);

			// Draw cursor
			int mX = mouse_pos.x;
			int mY = mouse_pos.y;

			if (materials[brush_material].id == 0 || materials[brush_material].id == 3) {
				DrawRectangleLines(mX-brush_size, mY-brush_size, brush_size*2, brush_size*2, G_WHITE);
			} else {
				DrawCircleLines(mX, mY, brush_size, G_WHITE);
			}

			DrawLine(mX-4, mY, mX+3, mY, G_WHITE);
			DrawLine(mX, mY-4, mX, mY+3, G_WHITE);

			// Draw UI labels
			DrawTextF(FONT_UI, TextFormat("FPS: %i (%i)", GetFPS(), tick_count), 8, 0, FONT_UI.baseSize*0.75f, G_WHITE);
			DrawTextF(FONT_UI, TextFormat("Particles: %i", particle_count), 8, 24, FONT_UI.baseSize*0.75f, G_WHITE);
			DrawTextF(FONT_UI, TextFormat("Brush: %i", brush_size), 8, 48, FONT_UI.baseSize*0.75f, G_WHITE);
			DrawTextF(FONT_UI, TextFormat("Material: %s", material_names[brush_material]), 8, 72, FONT_UI.baseSize*0.75f, G_WHITE);
		EndDrawing();
	}

	// ------------------------------
	// De-initialize
	// ------------------------------
	CloseWindow();
	free(grid);

	return 0;
}

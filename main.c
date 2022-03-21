// main.c

#include "main.h"


int main() {
	// Initialization
	// ------------------------------

	// Init Window
	InitWindow(SCREEN_W, SCREEN_H, "SandSim");
	SetTargetFPS(600);
	srand(time(NULL));

	Vector2 mouse_pos;
	HideCursor();

	// Create Particle grid
	Particle(*grid)[] = malloc((GRID_WIDTH * GRID_HEIGHT) * sizeof(Particle));

	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			NewParticle(grid, 0, x, y);
		}
	}


	// Main Loop
	// ------------------------------

	while (!WindowShouldClose()) {
		// Mouse Input
		// ------------------------------

		mouse_pos = GetMousePosition();

		// Paint particles
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			PaintParticles(grid, brush_size, brush_density, brush_material, mouse_pos.x, mouse_pos.y);
		}

		// Resize brush
		if (GetMouseWheelMove()) {
			brush_size -= 1 * GetMouseWheelMove();
			if (brush_size < MIN_BRUSH_SIZE) { brush_size = MIN_BRUSH_SIZE; }
			if (brush_size > MAX_BRUSH_SIZE) { brush_size = MAX_BRUSH_SIZE; }
		}


		// Keyboard Input
		// ------------------------------

		// Change materials
		if (IsKeyPressed(KEY_ZERO))  { brush_material = 0; } // Empty
		if (IsKeyPressed(KEY_ONE))   { brush_material = 1; } // Sand
		if (IsKeyPressed(KEY_TWO))   { brush_material = 2; } // Water
		if (IsKeyPressed(KEY_THREE))  { brush_material = 3; } // Salt

		// Toggle spouts
		if (IsKeyPressed(KEY_S)) { spouts_enabled = !spouts_enabled; }

		// Erase Everything
		if (IsKeyPressed(KEY_BACKSPACE)) {
			for (int y = 0; y < GRID_HEIGHT; y++) {
				for (int x = 0; x < GRID_WIDTH; x++) {
					NewParticle(grid, 0, x, y);
				}
			}
		}

		// Update Everything
		// ------------------------------
	
		// Spouts (if enabled)
		if (spouts_enabled) {
			PaintParticles(grid, 10, 10, 1, SCREEN_W / 6, 0);
			PaintParticles(grid, 10, 10, 3, SCREEN_W - (SCREEN_W / 6), 0);
		}

		// Update Particles + Tick Count
		UpdateParticles(grid);
		tick_count++;


		// Draw Loop
		// ------------------------------
		BeginDrawing();
			ClearBackground(G_BLACK);

			// Draw Particles
			DrawParticles(grid);

			// Draw Brush/Cursor
			int mx = mouse_pos.x;
			int my = mouse_pos.y;

			if (materials[brush_material].id == 0) {
				DrawRectangleLines(mx-brush_size, my-brush_size, brush_size*2, brush_size*2, G_WHITE);
			} else {
				DrawCircleLines(mx, my, brush_size, G_WHITE);
			}

			DrawLine(mx-4, my, mx+3, my, G_WHITE);
			DrawLine(mx, my-4, mx, my+3, G_WHITE);

			// Particle Count
			int particle_count = GetParticleCount(grid);
			DrawText(TextFormat("Particles: %i", particle_count), 10, 10, 20, WHITE);
		EndDrawing();
	}

	// De-Initialization
	// ------------------------------

	free(grid);

	CloseWindow();
	return 0;
}

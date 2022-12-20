#include "main.h"
#include "element.h"
#include "menu.h"

int tick_count = 0;
int brush_size = 1;
int brush_element = 1;
float brush_density;
std::string element_name;

// Utility
//
bool RandRoll(int roll) {
	if (roll == 1) return true;
	if (roll == 0) return false;
	return (1 + rand() % roll) == roll;
}

bool RandChance(float chance) {
	if (chance == 1.0) return true;
	if (chance == 0.0) return false;
	float roll = ((float)rand()) / (float)RAND_MAX;
	return ceil(roll * 100) / 100 <= chance;
}

int RandRange(int min, int max) {
	return rand() % (max + 1 - min) + min;
}

void DrawTextShadow(const char* text, int x, int y, int shadow_offset_x, int shadow_offset_y, int font_size, Color text_color, Color shadow_color) {
	DrawText(text, x + shadow_offset_x, y + shadow_offset_y, font_size, shadow_color);
	DrawText(text, x, y, font_size, text_color);
}

int ClampInt(int value, int min, int max) {
	const int number = value < min ? min : value;
	return number > max ? max : number;
}

Color OffsetColor(Color color, int min, int max) {
	int offset = RandRange(min, max);
	Color new_color = color;
	new_color.r = ClampInt(new_color.r + offset, 15, 255);
	new_color.g = ClampInt(new_color.g + offset, 15, 255);
	new_color.b = ClampInt(new_color.b + offset, 15, 255);
	return new_color;
}

void SetBrush(unsigned int id) {
	element_name = GetElement(brush_element)->name;
	brush_density = 0.05;

	if (GetElement(brush_element)->type == TypeSolid)
		brush_density = 1.0;
}


// Main Loop
//
int main() {
	InitWindow(SCREEN_W, SCREEN_H, "SandSimPlusPlus");
	SetTargetFPS(166);
	srand((int)time(NULL));
	HideCursor();

	// Preferences
	bool show_grid = false;

	// Sizing
	constexpr int grid_w = SCREEN_W / SCALE;
	constexpr int grid_h = SCREEN_H / SCALE;

	// Texture
	Image backBuffer = GenImageColor(grid_w, grid_h, BLACK);
	Texture bufferTexture = LoadTextureFromImage(backBuffer);

	// Elements
	ElementGrid grid(grid_w, grid_h);
	RegisterElements();

	for (int x = 0; x < grid_w; x++) {
		for (int y = 0; y < grid_h; y++) {
			Vector2i pos = { x, y };
			ElementData& data = grid.Get(pos);
			GetElement(AIR)->Create(grid, data, pos);
		}
	}

	SetBrush(brush_element);

	// Main Loop
	//
	while (!WindowShouldClose()) {
		// Update
		//
		tick_count++;

		int mouse_x = (int)GetMouseX();
		int mouse_y = (int)GetMouseY();

		int mouse_wheel = (int)GetMouseWheelMove();

		// Sand Spout
		// if (RandChance(0.75)) {
		// 	Vector2i pos = { (grid_w / 2) + RandRange(-2, 2), 0 };
		// 	ElementData& data = grid.Get(pos);
		// 	GetElement(SAND)->Create(grid, data, pos);
		// }

		// Brush Painting
		int brush_x = mouse_x / SCALE;
		int brush_y = mouse_y / SCALE;

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			int bx = brush_x - (brush_size / 2);
			int by = brush_y - (brush_size / 2);

			for (int x = bx; x < bx + brush_size; x++) {
				for (int y = by; y < by + brush_size; y++) {
					Vector2i pos = { x, y };
					if (RandChance(brush_density) && grid.IsEmpty(pos)) {
						ElementData& data = grid.Get(pos);
						GetElement(brush_element)->Create(grid, data, pos);
					}
				}
			}
		}

		// Brush Erasing
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			int bx = brush_x - (brush_size / 2);
			int by = brush_y - (brush_size / 2);

			for (int x = bx; x < bx + brush_size; x++) {
				for (int y = by; y < by + brush_size; y++) {
					Vector2i pos = { x, y };
					if (grid.InBounds(pos) && !grid.IsEmpty(pos)) {
						ElementData& data = grid.Get(pos);
						GetElement(AIR)->Create(grid, data, pos);
					}
				}
			}
		}

		// Brush Size
		if (IsKeyDown(KEY_LEFT_SHIFT))
			mouse_wheel *= 10;

		brush_size -= round(mouse_wheel);

		if (brush_size > MAX_BRUSH) {
			brush_size = MAX_BRUSH;
		}

		if (brush_size < MIN_BRUSH) {
			brush_size = MIN_BRUSH;
		}

		// Next/Previous Brush Element
		if (IsKeyPressed(KEY_W) && brush_element < 8) {
			brush_element++;
			SetBrush(brush_element);
		}

		if (IsKeyPressed(KEY_S) && brush_element > SAND) {
			brush_element--;
			SetBrush(brush_element);
		}

		// Element Grid
		bool is_even_tick = ((tick_count % 2) == 0);
		int dir = is_even_tick ? 0 : 1;

		for (int y = grid_h - 1; y >= 0; y--) {
			for (int x = dir ? 0 : grid_w - 1; dir ? x < grid_w : x > 0; dir ? x++ : x--) {
				Vector2i pos = { x, y };
				ElementData& data = grid.Get(pos);
				if (data.id != AIR)
					GetElement(data.id)->Update(grid, data, pos);
			}
		}

		// Texture
		ImageClearBackground(&backBuffer, BLACK);

		for (int y = 0; y < grid_h; y++) {
			for (int x = 0; x < grid_w; x++) {
				Vector2i pos = { x, y };
				ElementData& data = grid.Get(pos);
				if (data.id != AIR)
					// ImageDrawPixel(&backBuffer, pos.x, pos.y, GetElement(data.id)->GetColor(grid, data, pos));
					ImageDrawPixel(&backBuffer, pos.x, pos.y, data.color);
			}
		}

		UpdateTexture(bufferTexture, backBuffer.data);

		// Draw
		//
		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexturePro(bufferTexture, Rectangle { 0, 0, grid_w, grid_h }, Rectangle { 0, 0, SCREEN_W, SCREEN_H }, Vector2 { 0, 0 }, 0, WHITE);

		// Grid
		if (show_grid) {
			for (int x = 0; x < grid_w; x++)
				DrawLine(x * SCALE, 0, x * SCALE, SCREEN_H, Color { 255, 255, 255, 50 });

			for (int y = 0; y < grid_h; y++)
				DrawLine(0, y * SCALE, SCREEN_W, y * SCALE, Color { 255, 255, 255, 50 });
		}

		// Overlay
		Color fps_color = GREEN;

		if (GetFPS() < 100)
			fps_color = YELLOW;

		if (GetFPS() < 60)
			fps_color = RED;

		const char* fps_text = TextFormat("%i FPS", GetFPS());
		DrawTextShadow(fps_text, SCREEN_W - MeasureText(fps_text, 20) - 6, 6, 1, 1, 20, fps_color, BLACK);

		DrawTextShadow(TextFormat("Element: %s", element_name.c_str()), 10, 10, 1, 1, 20, WHITE, DARKGRAY);

		// Brush
		DrawRectangleLines(((brush_x * SCALE) - (brush_size / 2) * SCALE) + 1, ((brush_y * SCALE) - (brush_size / 2) * SCALE) + 1, brush_size * SCALE, brush_size * SCALE, DARKGRAY);
		DrawRectangleLines((brush_x * SCALE) - (brush_size / 2) * SCALE, (brush_y * SCALE) - (brush_size / 2) * SCALE, brush_size * SCALE, brush_size * SCALE, WHITE);

		EndDrawing();
	}

	// End Program
	//
	UnloadTexture(bufferTexture);
	CloseWindow();
	return 0;
}
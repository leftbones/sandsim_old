#include "main.h"
#include "element.h"

using namespace std;


// Utility
//
bool RandChance(float chance) {
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


// Main
//
int main() {
    InitWindow(SCREEN_W, SCREEN_H, "SandSim");
    SetTargetFPS(120);
    srand(time(NULL));
    HideCursor();

    int tick_count = 0;
    int brush_element = 1;
    int brush_size = 15;
    float brush_density = 0.75;

    // Elements
    RegisterElements();
    ElementGrid grid(GRID_W, GRID_H);

    // Texture
    Image backBuffer = GenImageColor(GRID_W, GRID_H, BLACK);
    Texture bufferTexture = LoadTextureFromImage(backBuffer);


    // Main Loop
    //
    while (!WindowShouldClose()) {
        // Update
        //
        tick_count++;

        int mouse_x = (int)GetMouseX();
        int mouse_y = (int)GetMouseY();

        // Element Grid
        grid.DoForEach([](ElementData& data, ElementGrid& grid, Vector2i& pos) {
            GetElement(data.id)->Update(data, grid, pos);
        });

        // Sand Spout
        if (RandChance(0.75)) {
            Vector2i pos { (GRID_W / 2) + RandRange(-2, 2), 0 };
            ElementData& data = grid.Get(pos);
            GetElement(WATER)->Create(data, grid, pos);
        }

        // Brush Painting
        int brush_x = (mouse_x / SCALE);
        int brush_y = (mouse_y / SCALE);

        brush_density = min(15, brush_size * 2);

        // Brush Sizing
        int mouse_wheel = (int)GetMouseWheelMove();
        if (IsKeyDown(KEY_LEFT_SHIFT))
            mouse_wheel *= 10;

        brush_size += round(mouse_wheel);

        if (brush_size > MAX_BRUSH)
            brush_size = MAX_BRUSH;

        if (brush_size < MIN_BRUSH)
            brush_size = MIN_BRUSH;

        // Update Texture
        ImageClearBackground(&backBuffer, BLACK);

        grid.DoForEach([&backBuffer](ElementData& data, ElementGrid& grid, Vector2i& pos) {
            Color c = GetElement(data.id)->GetColor(data, grid, pos);
            ImageDrawPixel(&backBuffer, pos.x, pos.y, c);
        });

        UpdateTexture(bufferTexture, backBuffer.data);

        // Draw
        //
        BeginDrawing();
        ClearBackground(BLACK);

        // Texture
        DrawTexturePro(bufferTexture, Rectangle {0, 0, float(bufferTexture.width), float(bufferTexture.height) }, Rectangle { 0, 0, SCREEN_W, SCREEN_H }, Vector2 { 0, 0 }, 0, WHITE);

        // Brush
        DrawRectangleLines(((brush_x * SCALE) - (brush_size / 2) * SCALE) + 1, ((brush_y * SCALE) - (brush_size / 2) * SCALE) + 1, brush_size * SCALE, brush_size * SCALE, DARKGRAY);
        DrawRectangleLines(((brush_x * SCALE) - (brush_size / 2) * SCALE), ((brush_y * SCALE) - (brush_size / 2) * SCALE), brush_size * SCALE, brush_size * SCALE, WHITE);

        // Info
        DrawTextShadow(TextFormat("BRUSH: %i", brush_size), 10, 10, 1, 1, 20, WHITE, DARKGRAY);

        DrawTextShadow(TextFormat("FPS: %i", GetFPS()), 10, SCREEN_H - 30, 1, 1, 20, WHITE, DARKGRAY);
        EndDrawing();
    }

    // Program Exit
    //
    CloseWindow();
    return 0;
}

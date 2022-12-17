#include "main.h"
#include "element.h"

using namespace std;

int tick_count = 0;
int brush_element = 1;
int brush_size = 15;
float brush_density = 20;


// Utility
//
bool RandChance(float chance) {
    float roll = ((float)rand()) / (float)RAND_MAX;
    return ceil(roll * 100) / 100 <= chance;
}

bool RandRoll(int roll) {
    if (roll == 0) return false;
    return (1 + rand() % roll) == roll;
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
    SetTargetFPS(166);
    srand(time(NULL));
    HideCursor();

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

        int mouse_wheel = (int)GetMouseWheelMove();

        // Spout
        if (RandRoll(2)) {
            Vector2i pos { (GRID_W / 2) + RandRange(-2, 2), 0 };
            ElementData& data = grid.Get(pos);
            GetElement(SAND)->Create(data, grid, pos);
        }

        // Brush Painting
        int brush_x = (mouse_x / SCALE);
        int brush_y = (mouse_y / SCALE);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int bx = brush_x - (brush_size / 2);
            int by = brush_y - (brush_size / 2);

            for (int x = bx; x < bx + brush_size; x++) {
                for (int y = by; y < by + brush_size; y++) {
                    if (RandRoll(brush_density)) {
                        Vector2i pos {x, y};
                        ElementData& data = grid.Get(pos);
                        GetElement(brush_element)->Create(data, grid, pos);
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
                    Vector2i pos {x, y};
                    ElementData& data = grid.Get(pos);
                    data.id = NONE;
                }
            }
        }

        // Brush Sizing
        if (IsKeyDown(KEY_LEFT_SHIFT))
            mouse_wheel *= 10;

        brush_size -= round(mouse_wheel);

        if (brush_size > MAX_BRUSH)
            brush_size = MAX_BRUSH;

        if (brush_size < MIN_BRUSH)
            brush_size = MIN_BRUSH;

        // Element Selection
        if (IsKeyDown(KEY_Q) && brush_element > SAND)
            brush_element--;

        if (IsKeyDown(KEY_E) && brush_element < WATER)
            brush_element++;

        // Element Grid
        grid.DoForEach([](ElementData& data, ElementGrid& grid, Vector2i& pos) {
            GetElement(data.id)->Update(data, grid, pos);
        });

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
        DrawTexturePro(bufferTexture, Rectangle {0, 0, GRID_W, GRID_H }, Rectangle { 0, 0, SCREEN_W, SCREEN_H }, Vector2 { 0, 0 }, 0, WHITE);

        // Cursor/Brush
        DrawRectangleLines(((brush_x * SCALE) - (brush_size / 2) * SCALE) + 1, ((brush_y * SCALE) - (brush_size / 2) * SCALE) + 1, brush_size * SCALE, brush_size * SCALE, DARKGRAY);
        DrawRectangleLines(((brush_x * SCALE) - (brush_size / 2) * SCALE), ((brush_y * SCALE) - (brush_size / 2) * SCALE), brush_size * SCALE, brush_size * SCALE, WHITE);

        // Info
        DrawTextShadow(TextFormat("BRUSH: %i", brush_size), 10, 10, 1, 1, 20, WHITE, DARKGRAY);
        DrawTextShadow(TextFormat("ELEMENT: %s", GetElement(brush_element)->name.c_str()), 10, 30, 2, 2, 20, WHITE, DARKGRAY);
        DrawTextShadow(TextFormat("X: %i Y: %i", mouse_x, mouse_y), 10, 50, 2, 2, 20, WHITE, DARKGRAY);

        DrawTextShadow(TextFormat("FPS: %i", GetFPS()), 10, SCREEN_H - 30, 2, 2, 20, WHITE, DARKGRAY);
        EndDrawing();
    }

    // Program Exit
    //
    CloseWindow();
    return 0;
}

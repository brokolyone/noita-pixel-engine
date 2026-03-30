#include <raylib.h>
#include "noita_core.hpp"
#include <vector>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int worldWidth = 200; // Низкое разрешение для эффекта "пиксель-арт"
    const int worldHeight = 150;

    InitWindow(screenWidth, screenHeight, "Noita-like Engine Prototype");
    SetTargetFPS(60);

    // Ждем, пока окно реально инициализируется (важно для Android)
    while (!IsWindowReady()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    Noita::World world(worldWidth, worldHeight);
    
    // ... остальной код текстуры ...
    Image image = {
        .data = (void*)world.GetPixels(),
        .width = worldWidth,
        .height = worldHeight,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    Texture2D texture = LoadTextureFromImage(image);

    Noita::ParticleType currentType = Noita::ParticleType::SAND;
    int brushRadius = 3;

    while (!WindowShouldClose()) {
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // Управление: рисуем частицы (защита от деления на ноль)
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && sw > 0 && sh > 0) {
            Vector2 mousePos = GetMousePosition();
            int x = (int)(mousePos.x / sw * worldWidth);
            int y = (int)(mousePos.y / sh * worldHeight);
            world.SetParticleCircle(x, y, brushRadius, currentType);
        }

        // Изменение размера кисти
        if (IsKeyPressed(KEY_RIGHT_BRACKET)) brushRadius++;
        if (IsKeyPressed(KEY_LEFT_BRACKET) && brushRadius > 0) brushRadius--;

        // Переключение типов
        if (IsKeyPressed(KEY_ONE)) currentType = Noita::ParticleType::SAND;
        if (IsKeyPressed(KEY_TWO)) currentType = Noita::ParticleType::WATER;
        if (IsKeyPressed(KEY_THREE)) currentType = Noita::ParticleType::WALL;
        if (IsKeyPressed(KEY_FOUR)) currentType = Noita::ParticleType::AIR;
        if (IsKeyPressed(KEY_FIVE)) currentType = Noita::ParticleType::FIRE;

        // Симуляция
        world.Update();

        // Рендеринг
        BeginDrawing();
        ClearBackground(BLACK);

        // Обновляем текстуру новыми пикселями из мира
        UpdateTexture(texture, world.GetPixels());

        // Рисуем текстуру растянутой на весь экран
        if (sw > 0 && sh > 0) {
            DrawTexturePro(texture, 
                { 0, 0, (float)worldWidth, (float)worldHeight },
                { 0, 0, (float)sw, (float)sh },
                { 0, 0 }, 0.0f, WHITE);
        }

        DrawText(TextFormat("Brush: %d | 1-5: Type | []: Size", brushRadius), 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}

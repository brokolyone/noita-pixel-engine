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

    Noita::World world(worldWidth, worldHeight);

    // Текстура для отображения пикселей мира
    Image image = {
        .data = (void*)world.GetPixels(),
        .width = worldWidth,
        .height = worldHeight,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    Texture2D texture = LoadTextureFromImage(image);

    Noita::ParticleType currentType = Noita::ParticleType::SAND;

    while (!WindowShouldClose()) {
        // Управление: рисуем частицы
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int x = (int)(mousePos.x / screenWidth * worldWidth);
            int y = (int)(mousePos.y / screenHeight * worldHeight);
            world.SetParticle(x, y, currentType);
        }

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
        DrawTexturePro(texture, 
            { 0, 0, (float)worldWidth, (float)worldHeight },
            { 0, 0, (float)screenWidth, (float)screenHeight },
            { 0, 0 }, 0.0f, WHITE);

        DrawText("1: Sand | 2: Water | 3: Wall | 4: Erase | 5: Fire", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}

#include "noita_core.hpp"
#include <random>

namespace Noita {

    World::World(int width, int height) : m_width(width), m_height(height) {
        m_particles.resize(width * height);
        m_pixelBuffer.resize(width * height);
    }

    World::~World() {}

    void World::Update() {
        // Очищаем флаг обновления для всех частиц
        for (auto& p : m_particles) p.updated = false;

        // Идем снизу вверх (чтобы песок падал корректно)
        for (int y = m_height - 1; y >= 0; --y) {
            // Случайное направление по X (влево или вправо), чтобы не было смещения в одну сторону
            bool leftToRight = (rand() % 2 == 0);
            if (leftToRight) {
                for (int x = 0; x < m_width; ++x) {
                    auto& p = m_particles[y * m_width + x];
                    if (p.updated || p.type == ParticleType::AIR || p.type == ParticleType::WALL) continue;
                    if (p.type == ParticleType::SAND) UpdateSand(x, y);
                    else if (p.type == ParticleType::WATER) UpdateWater(x, y);
                    else if (p.type == ParticleType::FIRE) UpdateFire(x, y);
                }
            } else {
                for (int x = m_width - 1; x >= 0; --x) {
                    auto& p = m_particles[y * m_width + x];
                    if (p.updated || p.type == ParticleType::AIR || p.type == ParticleType::WALL) continue;
                    if (p.type == ParticleType::SAND) UpdateSand(x, y);
                    else if (p.type == ParticleType::WATER) UpdateWater(x, y);
                    else if (p.type == ParticleType::FIRE) UpdateFire(x, y);
                }
            }
        }

        // Обновляем буфер пикселей для рендеринга
        for (int i = 0; i < m_width * m_height; ++i) {
            m_pixelBuffer[i] = GetColorForType(m_particles[i].type);
        }
    }

    void World::UpdateSand(int x, int y) {
        if (y + 1 >= m_height) return;

        // Прямо вниз
        if (IsEmpty(x, y + 1)) {
            MoveParticle(x, y, x, y + 1);
        }
        // Вниз-влево
        else if (x > 0 && IsEmpty(x - 1, y + 1)) {
            MoveParticle(x, y, x - 1, y + 1);
        }
        // Вниз-вправо
        else if (x + 1 < m_width && IsEmpty(x + 1, y + 1)) {
            MoveParticle(x, y, x + 1, y + 1);
        }
    }

    void World::UpdateWater(int x, int y) {
        if (y + 1 >= m_height) return;

        // Прямо вниз
        if (IsEmpty(x, y + 1)) {
            MoveParticle(x, y, x, y + 1);
        }
        // Вниз-влево
        else if (x > 0 && IsEmpty(x - 1, y + 1)) {
            MoveParticle(x, y, x - 1, y + 1);
        }
        // Вниз-вправо
        else if (x + 1 < m_width && IsEmpty(x + 1, y + 1)) {
            MoveParticle(x, y, x + 1, y + 1);
        }
        // Растекаемся влево или вправо
        else {
            bool moveLeft = (rand() % 2 == 0);
            if (moveLeft && x > 0 && IsEmpty(x - 1, y)) {
                MoveParticle(x, y, x - 1, y);
            } else if (x + 1 < m_width && IsEmpty(x + 1, y)) {
                MoveParticle(x, y, x + 1, y);
            }
        }
    }

    void World::UpdateFire(int x, int y) {
        // Огонь иногда тухнет сам по себе
        if (rand() % 10 == 0) {
            m_particles[y * m_width + x].type = ParticleType::AIR;
            return;
        }

        if (y <= 0) return;

        // Прямо вверх
        if (IsEmpty(x, y - 1)) {
            MoveParticle(x, y, x, y - 1);
        }
        // Вверх-влево
        else if (x > 0 && IsEmpty(x - 1, y - 1)) {
            MoveParticle(x, y, x - 1, y - 1);
        }
        // Вверх-вправо
        else if (x + 1 < m_width && IsEmpty(x + 1, y - 1)) {
            MoveParticle(x, y, x + 1, y - 1);
        }
    }

    bool World::IsEmpty(int x, int y) {
        return m_particles[y * m_width + x].type == ParticleType::AIR;
    }

    void World::MoveParticle(int x1, int y1, int x2, int y2) {
        auto& p1 = m_particles[y1 * m_width + x1];
        auto& p2 = m_particles[y2 * m_width + x2];
        p2 = p1;
        p2.updated = true;
        p1 = { ParticleType::AIR, 0x00000000, false };
    }

    void World::SetParticle(int x, int y, ParticleType type) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
        m_particles[y * m_width + x] = { type, GetColorForType(type), false };
    }

    void World::SetParticleCircle(int x, int y, int radius, ParticleType type) {
        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                if (i * i + j * j <= radius * radius) {
                    SetParticle(x + i, y + j, type);
                }
            }
        }
    }

    const uint32_t* World::GetPixels() const {
        return m_pixelBuffer.data();
    }

    uint32_t World::GetColorForType(ParticleType type) {
        switch (type) {
            case ParticleType::AIR: return 0x222222FF; // Темно-серый фон
            case ParticleType::SAND: return 0xEEDD88FF; // Желтый песок
            case ParticleType::WATER: return 0x3366FFFF; // Синяя вода
            case ParticleType::WALL: return 0x888888FF; // Серые стены
            case ParticleType::FIRE: return 0xFF4400FF; // Красный огонь
            default: return 0x000000FF;
        }
    }

} // namespace Noita

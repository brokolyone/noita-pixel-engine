#ifndef NOITA_CORE_HPP
#define NOITA_CORE_HPP

#include <vector>
#include <cstdint>

#ifdef _WIN32
    #ifdef NOITA_CORE_EXPORTS
        #define NOITA_API __declspec(dllexport)
    #else
        #define NOITA_API __declspec(dllimport)
    #endif
#else
    #define NOITA_API __attribute__((visibility("default")))
#endif

namespace Noita {

    enum class ParticleType : uint8_t {
        AIR = 0,
        SAND = 1,
        WATER = 2,
        WALL = 3,
        FIRE = 4
    };

    struct Particle {
        ParticleType type = ParticleType::AIR;
        uint32_t color = 0x00000000; // RGBA
        bool updated = false; // Чтобы не обновлять одну частицу дважды за кадр
    };

    class NOITA_API World {
    public:
        World(int width, int height);
        ~World();

        void Update();
        void SetParticle(int x, int y, ParticleType type);
        const uint32_t* GetPixels() const;

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        int m_width;
        int m_height;
        std::vector<Particle> m_particles;
        std::vector<uint32_t> m_pixelBuffer; // Буфер для рендеринга

        void UpdateSand(int x, int y);
        void UpdateWater(int x, int y);
        void UpdateFire(int x, int y);
        bool IsEmpty(int x, int y);
        void MoveParticle(int x1, int y1, int x2, int y2);
        uint32_t GetColorForType(ParticleType type);
    };

} // namespace Noita

#endif // NOITA_CORE_HPP

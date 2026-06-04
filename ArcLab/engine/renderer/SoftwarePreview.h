#pragma once

#include "engine/math/Color.h"
#include "engine/physics/Particle.h"
#include "engine/renderer/DebugDraw.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

namespace arclab {

class SoftwarePreview {
public:
    SoftwarePreview(int width = 1280, int height = 720)
        : width_(width)
        , height_(height)
        , pixels_(static_cast<std::size_t>(width * height), Color::black())
        , rgba8_(static_cast<std::size_t>(width * height), 0)
    {
    }

    void clear(const Color& color = { 0.02f, 0.025f, 0.03f, 1.0f })
    {
        std::fill(pixels_.begin(), pixels_.end(), color);
    }

    void draw_debug_lines(const DebugDrawList& drawList)
    {
        for (const DebugLine& line : drawList.lines()) {
            draw_line(world_to_pixel(line.start), world_to_pixel(line.end), line.color);
        }
    }

    void draw_particles(const ParticleSystem& particles)
    {
        for (const Particle& particle : particles.particles()) {
            const double speed = particle.velocity.length();
            const Color color = lerp(Color::electric_blue(), Color::magnetic_red(), clamp(speed / 12.0, 0.0, 1.0));
            draw_disc(world_to_pixel(particle.position), 2, color);
        }
    }

    bool save_ppm(const std::filesystem::path& path) const
    {
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        file << "P6\n" << width_ << ' ' << height_ << "\n255\n";
        for (const Color& pixel : pixels_) {
            const unsigned char r = to_byte(pixel.r);
            const unsigned char g = to_byte(pixel.g);
            const unsigned char b = to_byte(pixel.b);
            file.write(reinterpret_cast<const char*>(&r), 1);
            file.write(reinterpret_cast<const char*>(&g), 1);
            file.write(reinterpret_cast<const char*>(&b), 1);
        }

        return true;
    }

    int width() const
    {
        return width_;
    }

    int height() const
    {
        return height_;
    }

    const std::vector<std::uint32_t>& rgba8_pixels()
    {
        for (std::size_t i = 0; i < pixels_.size(); ++i) {
            const Color& pixel = pixels_[i];
            const std::uint32_t r = to_byte(pixel.r);
            const std::uint32_t g = to_byte(pixel.g);
            const std::uint32_t b = to_byte(pixel.b);
            rgba8_[i] = (r << 16u) | (g << 8u) | b;
        }
        return rgba8_;
    }

private:
    struct PixelPoint {
        int x = 0;
        int y = 0;
    };

    static unsigned char to_byte(float value)
    {
        return static_cast<unsigned char>(clamp(static_cast<double>(value), 0.0, 1.0) * 255.0 + 0.5);
    }

    PixelPoint world_to_pixel(const Vec3& world) const
    {
        constexpr double viewHalfExtent = 10.0;
        const double u = (world.x + viewHalfExtent) / (viewHalfExtent * 2.0);
        const double v = (world.z + viewHalfExtent) / (viewHalfExtent * 2.0);
        return {
            static_cast<int>(u * static_cast<double>(width_ - 1)),
            static_cast<int>((1.0 - v) * static_cast<double>(height_ - 1)),
        };
    }

    void set_pixel(int x, int y, const Color& color)
    {
        if (x < 0 || y < 0 || x >= width_ || y >= height_) {
            return;
        }

        pixels_[static_cast<std::size_t>(y * width_ + x)] = color;
    }

    void draw_disc(PixelPoint center, int radius, const Color& color)
    {
        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                if (x * x + y * y <= radius * radius) {
                    set_pixel(center.x + x, center.y + y, color);
                }
            }
        }
    }

    void draw_line(PixelPoint start, PixelPoint end, const Color& color)
    {
        int x0 = start.x;
        int y0 = start.y;
        const int x1 = end.x;
        const int y1 = end.y;
        const int dx = std::abs(x1 - x0);
        const int sx = x0 < x1 ? 1 : -1;
        const int dy = -std::abs(y1 - y0);
        const int sy = y0 < y1 ? 1 : -1;
        int error = dx + dy;

        for (;;) {
            set_pixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) {
                break;
            }

            const int e2 = 2 * error;
            if (e2 >= dy) {
                error += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                error += dx;
                y0 += sy;
            }
        }
    }

    int width_ = 1280;
    int height_ = 720;
    std::vector<Color> pixels_;
    std::vector<std::uint32_t> rgba8_;
};

} // namespace arclab

#pragma once

#include "engine/math/Math.h"
#include "engine/math/Vector.h"

#include <cstdint>
#include <vector>

namespace arclab {

struct Vertex {
    Vec3 position;
    Vec3 normal { 0.0, 1.0, 0.0 };
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    static Mesh make_line_grid(double halfExtent = 10.0, double spacing = 1.0)
    {
        Mesh mesh;
        for (double p = -halfExtent; p <= halfExtent + kEpsilon; p += spacing) {
            const std::uint32_t base = static_cast<std::uint32_t>(mesh.vertices.size());
            mesh.vertices.push_back({ { -halfExtent, 0.0, p } });
            mesh.vertices.push_back({ { halfExtent, 0.0, p } });
            mesh.vertices.push_back({ { p, 0.0, -halfExtent } });
            mesh.vertices.push_back({ { p, 0.0, halfExtent } });
            mesh.indices.insert(mesh.indices.end(), { base, base + 1, base + 2, base + 3 });
        }
        return mesh;
    }
};

} // namespace arclab

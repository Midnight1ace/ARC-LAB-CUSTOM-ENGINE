#pragma once

#include "engine/math/Math.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

namespace arclab {

class PerspectiveCamera {
public:
    void set_viewport(double width, double height)
    {
        viewportWidth_ = width <= 0.0 ? 1.0 : width;
        viewportHeight_ = height <= 0.0 ? 1.0 : height;
    }

    void set_position(const Vec3& position)
    {
        position_ = position;
    }

    void look_at_point(const Vec3& target)
    {
        target_ = target;
    }

    void orbit(double yawDeltaRadians, double pitchDeltaRadians, double zoomDelta)
    {
        yaw_ += yawDeltaRadians;
        pitch_ = clamp(pitch_ + pitchDeltaRadians, radians(-85.0), radians(85.0));
        distance_ = clamp(distance_ + zoomDelta, 1.0, 200.0);

        const double cp = std::cos(pitch_);
        position_ = target_ + Vec3 {
            distance_ * cp * std::sin(yaw_),
            distance_ * std::sin(pitch_),
            distance_ * cp * std::cos(yaw_),
        };
    }

    Mat4 view_matrix() const
    {
        return look_at(position_, target_, { 0.0, 1.0, 0.0 });
    }

    Mat4 projection_matrix() const
    {
        return perspective(verticalFovRadians_, viewportWidth_ / viewportHeight_, nearPlane_, farPlane_);
    }

    Vec3 position() const
    {
        return position_;
    }

private:
    Vec3 position_ { 0.0, 4.0, 10.0 };
    Vec3 target_ { 0.0, 0.0, 0.0 };
    double yaw_ = 0.0;
    double pitch_ = radians(20.0);
    double distance_ = 10.0;
    double verticalFovRadians_ = radians(60.0);
    double nearPlane_ = 0.01;
    double farPlane_ = 1000.0;
    double viewportWidth_ = 1280.0;
    double viewportHeight_ = 720.0;
};

} // namespace arclab

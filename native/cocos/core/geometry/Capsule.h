#pragma once

#include "core/geometry/Enums.h"
#include "math/Mat4.h"
#include "math/Quaternion.h"
#include "math/Utils.h"
#include "math/Vec3.h"

namespace cc {
namespace geometry {

/**
 * @en
 * Basic Geometry: capsule.
 * @zh
 * 基础几何，胶囊体。
 */
class Capsule final : public ShapeBase {
    /**
     * @en
     * Gets the type of the shape.
     * @zh
     * 获取形状的类型。
     */
public:
    enum class CenterEnum {
        X = 0,
        Y = 1,
        Z = 2
    };

    /**
     * @en
     * Capsule sphere radius.
     * @zh
     * 胶囊体球部半径。
     */
    float radius{0.0F};

    /**
     * @en
     * The distance between the center point of the capsule and the center of the sphere.
     * @zh
     * 胶囊体中心点和球部圆心的距离。
     */
    float halfHeight{0.0F};

    /**
     * @en
     * Local orientation of capsule [0,1,2] => [x,y,z].
     * @zh
     * 胶囊体的本地朝向，映射关系 [0,1,2] => [x,y,z]。
     */
    CenterEnum axis;

    /**
     * @en
     * The origin of the capsule.
     * @zh
     * 胶囊体的原点。
     */
    Vec3 center;

    /**
     * @en
     * The rotation of the capsule.
     * @zh
     * 胶囊体的旋转。
     */
    Quaternion rotation;

    /** cache, local center of ellipse */
    Vec3 ellipseCenter0;
    Vec3 ellipseCenter1;

    explicit Capsule(float radius = 0.5, float halfHeight = 0.5, CenterEnum axis = CenterEnum::Y) {
        this->setType(ShapeEnum::SHAPE_CAPSULE);
        this->radius     = radius;
        this->halfHeight = halfHeight;
        this->axis       = axis;
        this->center     = {};
        this->rotation   = {};
        ellipseCenter0   = {0, halfHeight, 0};
        ellipseCenter1   = {0, -halfHeight, 0};
        updateCache();
    }

    Capsule(const Capsule &other) = default;
    Capsule(Capsule &&other)      = default;
    Capsule &operator=(const Capsule &other) = default;
    Capsule &operator=(Capsule &&other) = default;
    ~Capsule() override                 = default;

    /**
     * @en
     * Transform this capsule.
     * @zh
     * 变换此胶囊体。
     */
    void transform(const Mat4 &m, const Vec3 &pos, const Quaternion &rot, const Vec3 &scale, Capsule *out) const;

private:
    void updateCache();
    void updateLocalCenter();
};
} // namespace geometry
} // namespace cc
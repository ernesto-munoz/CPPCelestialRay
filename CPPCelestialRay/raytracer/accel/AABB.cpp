#include "AABB.h"

const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);
const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);

const Interval& AABB::AxisInterval(int n) const
{
    if (n == 0) return x;
    if (n == 1) return y;
    return z;
}

bool AABB::Hit(const Ray& ray, Interval ray_t) const
{
    for (int axis = 0; axis < 3; ++axis) {
        const Interval& ax = AxisInterval(axis);
        const double adinv = 1.0 / ray.direction[axis];
        auto t0 = (ax.min - ray.origin[axis]) * adinv;
        auto t1 = (ax.max - ray.origin[axis]) * adinv;
        if (t0 < t1) {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        }
        else {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }
        if (ray_t.max <= ray_t.min) {
            return false;
        }
    }
    return true;
}

unsigned int AABB::LongestAxis() const
{
    if (x.Size() > y.Size()) {
        return x.Size() > z.Size() ? 0 : 2;
    }
    return y.Size() > z.Size() ? 1 : 2;
}

void AABB::PadMinimum()
{
    constexpr float delta = 1e-6;
    if (x.Size() < delta) x = x.Expand(delta);
    if (y.Size() < delta) y = y.Expand(delta);
    if (z.Size() < delta) z = z.Expand(delta);
}

AABB operator+(const AABB& bbox, const glm::vec3& offset)
{
    return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
}

AABB operator+(const glm::vec3& offset, const AABB& bbox)
{
    return bbox + offset;
}

AABB operator*(const AABB& bbox, const glm::mat4& matrix)
{
    glm::vec4 min = glm::vec4(bbox.x.min, bbox.z.min, bbox.z.min, 1.0f);
    glm::vec4 max = glm::vec4(bbox.x.max, bbox.z.max, bbox.z.max, 1.0f);
    min = matrix * min;
    max = matrix * max;
    return AABB(min, max);
}

AABB operator*(const glm::mat4& matrix, const AABB& bbox)
{
    glm::vec4 min = glm::vec4(bbox.x.min, bbox.z.min, bbox.z.min, 1.0f);
    glm::vec4 max = glm::vec4(bbox.x.max, bbox.z.max, bbox.z.max, 1.0f);
    min = min * matrix;
    max = min * matrix;
    return AABB(min, max);
}

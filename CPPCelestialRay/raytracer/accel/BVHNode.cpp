#include "BVHNode.h"

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    // select the axis with the larges bounding box of the span of objects
    bbox = AABB::empty;
    for (size_t object_index = start; object_index < end; ++object_index) {
        bbox = AABB(bbox, objects[object_index]->BBox());
    }
    int axis = bbox.LongestAxis();

    // select the correct comparasion function
    auto comparator = (axis == 0) ? BoxCompareX : (axis == 1) ? BoxCompareY : BoxCompareZ;

    size_t object_span = end - start;
    if (object_span == 1) {  // only one object left to insert from the oroginal list
        left = right = objects[start];  // those are shared_ptr, so safe and memory correct
    }
    else if (object_span == 2) {
        left = objects[start];
        right = objects[start+1];
    }
    else {
        // sort in the specified range
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid, end);
    }
}

bool BVHNode::Hit(const Ray& r, Interval ray_t, HitRecord& rec) const
{
    if (!bbox.Hit(r, ray_t)) {
        return false;
    }

    bool left_hit = left->Hit(r, ray_t, rec);
    bool right_hit = right->Hit(r, Interval(ray_t.min, left_hit ? rec.t : ray_t.max), rec);
    return left_hit || right_hit;
}

AABB BVHNode::BBox() const
{
    return bbox;
}

bool BVHNode::BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
    auto a_axis_interval = a->BBox().AxisInterval(axis);
    auto b_axis_interval = b->BBox().AxisInterval(axis);
    return a_axis_interval.min < b_axis_interval.min;
}

bool BVHNode::BoxCompareX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return BoxCompare(a, b, 0);
}

bool BVHNode::BoxCompareY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return BoxCompare(a, b, 1);
}

bool BVHNode::BoxCompareZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return BoxCompare(a, b, 2);
}

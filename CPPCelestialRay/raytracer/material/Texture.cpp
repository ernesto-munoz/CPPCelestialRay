#include "Texture.h"

Color SolidColor::GetColor(double u, double v, const glm::vec3& point) const
{
    return albedo;
}

Color CheckerTexture::GetColor(double u, double v, const glm::vec3& point) const
{
    int x = static_cast<int>(std::floor(inv_scale * point.x));
    int y = static_cast<int>(std::floor(inv_scale * point.y));
    int z = static_cast<int>(std::floor(inv_scale * point.z));

    bool is_even = (x + y + z) % 2 == 0;
    return is_even ? even->GetColor(u, v, point) : odd->GetColor(u, v, point);
}

Color ImageTexture::GetColor(double u, double v, const glm::vec3& point) const
{
    // return magenta as guide for an error
    if (color_buffer.raw_data == nullptr) { 
        return Color(1.0, 0.0, 1.0);
    }
    u = 1 - std::clamp(u, 0.0, 1.0);
    v = 1 - std::clamp(v, 0.0, 1.0); // flip v to image coordinates
    int i = static_cast<int>(u * color_buffer.width);
    int j = static_cast<int>(v * color_buffer.height);
    float color_scale = 1.0f / 255.0f;

    auto r = color_buffer.raw_data[(j * color_buffer.width + i) * color_buffer.channels];
    auto g = color_buffer.raw_data[(j * color_buffer.width + i) * color_buffer.channels + 1];
    auto b = color_buffer.raw_data[(j * color_buffer.width + i) * color_buffer.channels + 2];

    return Color(r * color_scale, g * color_scale, b * color_scale);
}

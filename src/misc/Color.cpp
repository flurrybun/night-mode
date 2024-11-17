#include "Color.hpp"

ccHSVValue color::rgb2hsv(const ccColor3B& rgb) {
    float r = std::clamp(static_cast<int>(rgb.r), 0, 255) / 255.0f;
    float g = std::clamp(static_cast<int>(rgb.g), 0, 255) / 255.0f;
    float b = std::clamp(static_cast<int>(rgb.b), 0, 255) / 255.0f;

    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;

    ccHSVValue hsv;
    hsv.v = max;

    if (delta < 0.00001f) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = (max > 0.0f) ? (delta / max) : 0.0f;

    if (r >= max) {
        hsv.h = (g - b) / delta;
    } else if (g >= max) {
        hsv.h = 2.0f + (b - r) / delta;
    } else {
        hsv.h = 4.0f + (r - g) / delta;
    }

    hsv.h *= 60.0f;
    if (hsv.h < 0.0f) {
        hsv.h += 360.0f;
    }

    return hsv;
}

ccColor3B color::hsv2rgb(const ccHSVValue& hsv) {
    float h = fmod(hsv.h, 360.0f);
    if (h < 0.0f) h += 360.0f;
    float s = std::clamp(hsv.s, 0.0f, 1.0f);
    float v = std::clamp(hsv.v, 0.0f, 1.0f);

    int i = static_cast<int>(h / 60.0f) % 6;
    float f = (h / 60.0f) - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);

    float r, g, b;
    switch (i) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }

    return ccc3(r * 255, g * 255, b * 255);
}

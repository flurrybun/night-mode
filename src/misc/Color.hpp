#pragma once

#include <Geode/cocos/include/ccTypes.h>
using namespace geode::prelude;

// hue is in degrees 0-360, saturation and value are a percentage 0-1

namespace color {
    ccHSVValue rgb2hsv(const ccColor3B&);
    ccColor3B hsv2rgb(const ccHSVValue&);
}

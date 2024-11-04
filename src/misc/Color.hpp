#pragma once

#include <Geode/cocos/include/ccTypes.h>
using namespace geode::prelude;

namespace color {
    ccHSVValue rgb2hsv(ccColor3B in);
    ccColor3B hsv2rgb(ccHSVValue in);
}
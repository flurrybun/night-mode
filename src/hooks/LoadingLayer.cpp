#include "../NightManager.hpp"
#include <Geode/modify/LoadingLayer.hpp>
using namespace geode::prelude;

class $modify(LoadingLayer) {
    $override
    bool init(bool p0) {
        if (!LoadingLayer::init(p0)) return false;
        if (!night::isNightMode()) return true;

        if (auto background = getChildByType<CCSprite>(0)) {
            background->setColor(night::adjustBrightness(ccc3(6, 20, 41)));
            background->setZOrder(-2);
        }

        return true;
    }
};

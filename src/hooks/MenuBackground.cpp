#include "../NightManager.hpp"
#include "../misc/Color.hpp"
using namespace geode::prelude;

// utility macros to change the background on init

#define MAKE_NIGHT_0(CLASS_NAME, BG_TYPE) \
class $modify(CLASS_NAME) { \
    bool init() { \
        if (!CLASS_NAME::init()) return false; \
        night::updateDefaultBackground(this, BG_TYPE); \
        return true; \
    } \
};

#define MAKE_NIGHT_1(CLASS_NAME, ARG1, BG_TYPE) \
class $modify(CLASS_NAME) { \
    bool init(ARG1 p0) { \
        if (!CLASS_NAME::init(p0)) return false; \
        night::updateDefaultBackground(this, BG_TYPE); \
        return true; \
    } \
};

#define MAKE_NIGHT_2(CLASS_NAME, ARG1, ARG2, BG_TYPE) \
class $modify(CLASS_NAME) { \
    bool init(ARG1 p0, ARG2 p1) { \
        if (!CLASS_NAME::init(p0, p1)) return false; \
        night::updateDefaultBackground(this, BG_TYPE); \
        return true; \
    } \
};

// the following layers simply use the default gradient background image

#include <Geode/modify/CreatorLayer.hpp>
MAKE_NIGHT_0(CreatorLayer, night::BGColor::Blue)

#include <Geode/modify/LevelBrowserLayer.hpp>
MAKE_NIGHT_1(LevelBrowserLayer, GJSearchObject*, night::BGColor::Blue)

#include <Geode/modify/LeaderboardsLayer.hpp>
MAKE_NIGHT_1(LeaderboardsLayer, LeaderboardState, night::BGColor::Blue)

#include <Geode/modify/LevelListLayer.hpp>
MAKE_NIGHT_1(LevelListLayer, GJLevelList*, night::BGColor::Blue)

#include <Geode/modify/LevelSearchLayer.hpp>
MAKE_NIGHT_1(LevelSearchLayer, int, night::BGColor::Blue)

#include <Geode/modify/EditLevelLayer.hpp>
MAKE_NIGHT_1(EditLevelLayer, GJGameLevel*, night::BGColor::Blue)

#include <Geode/modify/GauntletSelectLayer.hpp>
MAKE_NIGHT_1(GauntletSelectLayer, int, night::BGColor::Gray)

#include <Geode/modify/GauntletLayer.hpp>
MAKE_NIGHT_1(GauntletLayer, GauntletType, night::BGColor::Gray)

#include <Geode/modify/GJGarageLayer.hpp>
MAKE_NIGHT_0(GJGarageLayer, night::BGColor::Gray)

#include <Geode/modify/SecretLayer2.hpp>
MAKE_NIGHT_0(SecretLayer2, night::BGColor::Purple)

#include <Geode/modify/SecretLayer.hpp>
MAKE_NIGHT_0(SecretLayer, night::BGColor::Cyan)

#include <Geode/modify/SecretLayer4.hpp>
MAKE_NIGHT_0(SecretLayer4, night::BGColor::Red)

#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
auto getLILColor = [](GJGameLevel* level) {
    if (level->m_gauntletLevel) return night::BGColor::Gray; // gauntlet level
    if (level->m_dailyID.value() < 100000) return night::BGColor::Blue; // normal level
    if (level->m_dailyID.value() < 200000) return night::BGColor::Gray; // weekly demon
    return night::BGColor::Purple; // event level
};

MAKE_NIGHT_2(LevelInfoLayer, GJGameLevel*, bool, getLILColor(p0))

// the following layers change their color based on the current page

// since particles cannot change color, these layers don't have any stars.
// maybe it's possible by using a CCRenderTexture? but that's a lot of work
// for two layers that you don't go to very often...i think this will do

#include <Geode/modify/LevelSelectLayer.hpp>
class $modify(LevelSelectLayer) {
    $override
    ccColor3B colorForPage(int page) {
        if (!night::isNightMode()) return LevelSelectLayer::colorForPage(page);

        auto color = LevelSelectLayer::colorForPage(page % 9);
        if (page == 22) color = ccc3(88, 104, 113);

        auto hsv = color::rgb2hsv(color);

        hsv.s *= 0.85f;
        hsv.v *= 0.15f;

        return night::adjustBrightness(color::hsv2rgb(hsv));
    }
};

#include <Geode/modify/SecretRewardsLayer.hpp>
class $modify(SecretRewardsLayer) {
    // getPageColor is inlined, so we have to do things a little differently
    void adjustBackgroundColor() {
        // SecretRewardsLayer members are a bit messed up atm so we can't use m_backgroundSprite
        auto bg = typeinfo_cast<CCSprite*>(getChildByID("background"));
        if (!bg) return;

        auto hsv = color::rgb2hsv(bg->getColor());

        hsv.s *= 0.85f;
        hsv.v *= 0.4f;

        bg->setColor(night::adjustBrightness(color::hsv2rgb(hsv)));
    }

    $override
    bool init(bool p0) {
        if (!SecretRewardsLayer::init(p0)) return false;
        if (!night::isNightMode()) return true;

        adjustBackgroundColor();
        return true;
    }

    $override
    void scrollLayerMoved(CCPoint point) {
        SecretRewardsLayer::scrollLayerMoved(point);
        if (!night::isNightMode()) return;

        adjustBackgroundColor();
    }
};

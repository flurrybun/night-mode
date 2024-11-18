#pragma once

#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
using namespace geode::prelude;

namespace night {
    static const ccColor3B BLUE_BG_COLOR = ccc3(5, 17, 35);
    static const std::array<ccColor3B, 3> BLUE_PARTICLE_COLORS = {ccc3(96, 113, 168), ccc3(49, 100, 147), ccc3(1, 8, 20)};

    static const ccColor3B GRAY_BG_COLOR = ccc3(16, 16, 16);
    static const std::array<ccColor3B, 3> GRAY_PARTICLE_COLORS = {ccc3(140, 140, 140), ccc3(117, 117, 117), ccc3(4, 4, 4)};

    static const ccColor3B PURPLE_BG_COLOR = ccc3(19, 5, 35);
    static const std::array<ccColor3B, 3> PURPLE_PARTICLE_COLORS = {ccc3(163, 96, 168), ccc3(114, 49, 147), ccc3(10, 1, 17)};

    static const ccColor3B CYAN_BG_COLOR = ccc3(4, 19, 28);
    static const std::array<ccColor3B, 3> CYAN_PARTICLE_COLORS = {ccc3(96, 148, 168), ccc3(49, 106, 147), ccc3(1, 11, 20)};

    static const ccColor3B RED_BG_COLOR = ccc3(35, 5, 5);
    static const std::array<ccColor3B, 3> RED_PARTICLE_COLORS = {ccc3(168, 116, 96), ccc3(147, 57, 49), ccc3(20, 1, 1)};

    using ParticleCallback = std::function<void(CCParticleSystemQuad*)>;

    enum class BGColor {
        Blue,
        Gray,
        Purple,
        Cyan,
        Red
    };

    struct LayerInfo {
        std::string layerName;
        std::string bgSelector;
        BGColor bgColor;

        LayerInfo(const std::string& layerName, const std::string& bgSelector, BGColor bgColor = BGColor::Blue) :
            layerName(layerName),
            bgSelector(bgSelector),
            bgColor(bgColor) {}
    };

    bool isNightMode();

    void updateDefaultBackground(CCLayer*, BGColor);
    void updateExternalModBackground(CCLayer*, const std::string&, const LayerInfo&);

    void setBGColor(CCSprite*, BGColor);
    CCSprite* createBackgroundSprite();

    void addParticle(CCNode*, const std::string&, const std::string&, const std::optional<ParticleCallback>& = std::nullopt);
    void addParticlesWithColor(CCNode*, const std::array<ccColor3B, 3>&, const std::optional<ParticleCallback>& = std::nullopt);
    void addParticlesWithColor(CCNode*, BGColor, const std::optional<ParticleCallback>& = std::nullopt);
    void addGeodeParticles(CCNode*);

    ccColor3B adjustBrightness(const ccColor3B&, float = 1);
}

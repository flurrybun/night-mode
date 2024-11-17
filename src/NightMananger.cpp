#include "NightManager.hpp"
#include "misc/BrightnessSetting.hpp"
#include "misc/Color.hpp"
#include <Geode/binding/GameToolbox.hpp>

bool night::isNightMode() {
    return Mod::get()->getSettingValue<bool>("night-mode");
}

void night::updateDefaultBackground(CCLayer* self, BGColor bgColor) {
    if (!isNightMode()) return;

    CCSprite* background = typeinfo_cast<CCSprite*>(self->getChildByID("background"));
    if (!background) return;

    background->setZOrder(-10);

    setBGColor(background, bgColor);
    if (Mod::get()->getSettingValue<std::string>("background-stars") == "Show All") addParticlesWithColor(self, bgColor);
}

void night::updateExternalModBackground(CCLayer* self, const std::string& modID, const LayerInfo& layerInfo) {
    if (!isNightMode()) return;
    auto [layerName, bgSelector, bgColor] = layerInfo;

    std::optional<ParticleCallback> particleCallback = std::nullopt;

    CCSprite* background;
    if (bgSelector != "FIRST_OF_CCSPRITE") background = typeinfo_cast<CCSprite*>(self->querySelector(bgSelector));
    else background = self->getChildByType<CCSprite>(0);
    if (!background) return;

    background->setZOrder(-10);

    if (modID == "cvolton.betterinfo") {
        background->setVisible(false);

        background = createBackgroundSprite();
        self->addChild(background);
    } else if (layerName == "GlobedServersLayer" || layerName == "GlobedMenuLayer") {
        particleCallback = [](CCParticleSystemQuad* particle) {
            particle->setAngle(-180);
            particle->setAngleVar(0);
            particle->setSpeed(30);
            particle->setSpeedVar(3);
        };
    }

    setBGColor(background, bgColor);
    if (Mod::get()->getSettingValue<std::string>("background-stars") == "Show All") addParticlesWithColor(self, bgColor, particleCallback);
}

void night::setBGColor(CCSprite* bg, BGColor bgColor) {
    ccColor3B color = BLUE_BG_COLOR;

    if (bgColor == BGColor::Blue) color = BLUE_BG_COLOR;
    else if (bgColor == BGColor::Gray) color = GRAY_BG_COLOR;
    else if (bgColor == BGColor::Purple) color = PURPLE_BG_COLOR;
    else if (bgColor == BGColor::Cyan) color = CYAN_BG_COLOR;
    else if (bgColor == BGColor::Red) color = RED_BG_COLOR;

    bg->setColor(adjustBrightness(color));
}

CCSprite* night::createBackgroundSprite() {
    auto sprite = CCSprite::create("GJ_gradientBG.png");
    auto winSize = CCDirector::get()->getWinSize();
    sprite->setAnchorPoint({0, 0});
    sprite->setPosition({-5, -5});
    sprite->setScaleX((winSize.width + 10) / sprite->getContentSize().width);
    sprite->setScaleY((winSize.height + 10) / sprite->getContentSize().height);
    sprite->setZOrder(-11);

    return sprite;
}

CCParticleSystemQuad* night::addParticle(CCNode* parent, const std::string& id, const std::string& particleString, const std::optional<ParticleCallback>& callback) {
    auto particle = GameToolbox::particleFromString(particleString, nullptr, false);
    if (!particle) return nullptr;

    if (callback) callback.value()(particle);

    particle->setID(id);
    particle->setZOrder(-9);
    particle->setPosition(CCDirector::get()->getWinSize() / 2);
    parent->addChild(particle);

    if (particle->getEmissionRate() == -1) particle->setAutoRemoveOnFinish(true);

    return particle;
}

std::array<CCParticleSystemQuad*, 6> night::addParticlesWithColor(CCNode* parent, const std::array<ccColor3B, 3>& colors, const std::optional<ParticleCallback>& callback) {
    auto toStr = [](ccColor3B color) {
        color = adjustBrightness(color, 0.5);

        // don't vary color if it's grayscale

        float var = (color.r == color.g && color.g == color.b) ? 0 : (color.r + color.g + color.b) / 10 / 255;
        return std::to_string(color.r / 255.f) + "a" + std::to_string(var) + "a" +
               std::to_string(color.g / 255.f) + "a" + std::to_string(var) + "a" +
               std::to_string(color.b / 255.f) + "a" + std::to_string(var);
    };

    std::array<std::string, 3> strColors = { toStr(colors[0]), toStr(colors[1]), toStr(colors[2]) };
    std::array<CCParticleSystemQuad*, 6> particles;

    particles[0] = addParticle(parent, "init-big-stars"_spr, "10a0.1a4a2a-1a0a180a0a2a400a200a0a0a0a0a0a0a8a2a45a0a" + strColors[0] + "a0.8a0.3a0a0a0a0a" + strColors[0] + "a0.8a0.3a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);
    particles[1] = addParticle(parent, "big-stars"_spr, "15a-1a5a2a2a0a180a0a2a400a200a0a0a0a0a0a0a8a2a45a0a" + strColors[0] + "a0.8a0.3a0a0a0a0a" + strColors[0] + "a0.8a0.3a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);
    particles[2] = addParticle(parent, "init-small-stars"_spr, "70a0.1a4a2a-1a0a180a0a2a400a200a0a0a0a0a0a0a2a1a0a0a" + strColors[1] + "a0.6a0.4a0a0a0a0a" + strColors[1] + "a0.6a0.4a0a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);
    particles[3] = addParticle(parent, "small-stars"_spr, "105a-1a5a2a15a0a180a0a2a300a200a0a0a0a0a0a0a2a1a0a0a" + strColors[1] + "a0.6a0.4a0a0a0a0a" + strColors[1] + "a0.6a0.4a1a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);
    particles[4] = addParticle(parent, "init-space-dust"_spr, "30a0.1a4a2a-1a0a180a0a2a400a200a0a0a0a0a0a0a200a120a0a360a" + strColors[2] + "a0.19a0.1a0a0a0a0a" + strColors[2] + "a0.19a0.1a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);
    particles[5] = addParticle(parent, "space-dust"_spr, "55a-1a8a2a5a0a180a0a2a400a200a0a0a0a0a0a0a200a120a0a360a" + strColors[2] + "a0.19a0.1a0a0a0a0a" + strColors[2] + "a0.19a0.1a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0", callback);

    return particles;
}

std::array<CCParticleSystemQuad*, 6> night::addParticlesWithColor(CCNode* parent, BGColor color, const std::optional<ParticleCallback>& callback) {
    switch (color) {
        case BGColor::Blue:
            return addParticlesWithColor(parent, BLUE_PARTICLE_COLORS, callback);
        case BGColor::Gray:
            return addParticlesWithColor(parent, GRAY_PARTICLE_COLORS, callback);
        case BGColor::Purple:
            return addParticlesWithColor(parent, PURPLE_PARTICLE_COLORS, callback);
        case BGColor::Cyan:
            return addParticlesWithColor(parent, CYAN_PARTICLE_COLORS, callback);
        case BGColor::Red:
            return addParticlesWithColor(parent, RED_PARTICLE_COLORS, callback);
    }

    return addParticlesWithColor(parent, BLUE_PARTICLE_COLORS, callback);
}

void night::addGeodeParticles(CCNode* parent) {
    std::array<std::array<ccColor3B, 3>, 3> colors = {{
        {ccc3(168, 133, 96), ccc3(147, 89, 49), ccc3(17, 8, 1)},
        {ccc3(168, 96, 114), ccc3(147, 49, 63), ccc3(17, 1, 1)},
        {ccc3(161, 96, 168), ccc3(126, 49, 147), ccc3(17, 1, 11)}
    }};

    std::array<std::string, 3> ids = {"top-stars"_spr, "middle-stars"_spr, "bottom-stars"_spr};
    std::array<float, 3> positions = {140.f, 0.f, -140.f};

    for (int i = 0; i < 3; i++) {
        auto layer = CCLayer::create();
        layer->setID(ids[i]);
        layer->setPosition({0, positions[i]});
        layer->setZOrder(-9);
        parent->addChild(layer);

        auto color = colors[i];
        addParticlesWithColor(layer, color, [](CCParticleSystemQuad* particle) {
            particle->setPosVar({400, 75});
            particle->setTotalParticles(particle->getTotalParticles() * 0.33);
            particle->setEmissionRate(particle->getEmissionRate() * 0.33);
        });
    }
}

ccColor3B night::adjustBrightness(const ccColor3B& color, float scalar) {
    auto baseFactor = Mod::get()->getSettingValue<float>("adjust-brightness");
    if (baseFactor < 0) return color;

    auto factor = baseFactor + (baseFactor - 1) * (scalar - 1);
    if (factor == 1.0) return color;

    auto hsv = color::rgb2hsv(color);
    hsv.v *= factor;
    return color::hsv2rgb(hsv);
}

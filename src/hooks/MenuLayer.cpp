#include "../NightManager.hpp"
#include "../misc/Color.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/binding/GameManager.hpp>
using namespace geode::prelude;

// i don't know why i tried to update the colors manually like this,
// i should change this to simply reinitialize MenuGameLayer

CCAction* createHueShiftAction(ccColor3B defaultColor, float factor) {
    auto hueLeft = color::rgb2hsv(defaultColor);
    hueLeft.h -= 20 * factor;
    auto rgbHueLeft = color::hsv2rgb(hueLeft);

    auto hueRight = color::rgb2hsv(defaultColor);
    hueRight.h += 20 * factor;
    auto rgbHueRight = color::hsv2rgb(hueRight);

    return CCRepeatForever::create(CCSequence::create(
        CCTintTo::create(2, rgbHueRight.r, rgbHueRight.g, rgbHueRight.b),
        CCTintTo::create(2, defaultColor.r, defaultColor.g, defaultColor.b),
        CCTintTo::create(2, rgbHueLeft.r, rgbHueLeft.g, rgbHueLeft.b),
        CCTintTo::create(2, defaultColor.r, defaultColor.g, defaultColor.b),
        nullptr
    ));
}

class $modify(NightMenuGameLayer, MenuGameLayer) {
    struct Fields {
        ccColor3B m_bgColor;
        ccColor3B m_groundColor;
        ccColor3B m_lineColor;
        CCSprite* m_ground1;
        CCSprite* m_ground2;
        CCSprite* m_groundLine;

        std::vector<WeakRef<CCParticleSystemQuad>> m_particles;
        bool m_isUpdateColorScheduled = true;
    };
    
    void setGroundColor(CCSprite* ground, ccColor3B color) {
        ground->setColor(color);

        // this may seem useless, but removing this causes the ground to flash
        // the old blue color for a single frame on startup

        CCArrayExt<CCSprite*> groundSprites = ground->getChildren();
        for (auto groundSprite : groundSprites) {
            groundSprite->setColor(color);
        }
    }

    void createParticle(const std::string& id, bool shouldRepeat, const std::string& particleString) {
        auto particle = GameToolbox::particleFromString(particleString, nullptr, false);
        if (!particle) return;

        particle->setID(id);
        particle->setPosition(CCDirector::sharedDirector()->getWinSize() / 2 + ccp(0, 30));
        addChild(particle);

        if (!shouldRepeat) particle->setAutoRemoveOnFinish(true);
    }

    void initNightMode() {
        auto background = m_backgroundSprite;
        auto ground1 = m_fields->m_ground1;
        auto ground2 = m_fields->m_ground2;
        auto groundLine = m_fields->m_groundLine;
        
        background->setColor(m_fields->m_bgColor);
        background->runAction(createHueShiftAction(m_fields->m_bgColor, 0.6));
        if (ground1) {
            setGroundColor(ground1, m_fields->m_groundColor);
            ground1->runAction(createHueShiftAction(m_fields->m_groundColor, 0.6));
        }
        if (ground2) {
            setGroundColor(ground2, m_fields->m_groundColor);
            ground2->runAction(createHueShiftAction(m_fields->m_groundColor, 0.6));
        }
        if (groundLine) {
            groundLine->setColor(m_fields->m_lineColor);
            groundLine->runAction(createHueShiftAction(m_fields->m_lineColor, 0.6));
        }

        auto showParticles = Mod::get()->getSettingValue<std::string>("background-stars") != "Show None";
        if (showParticles && m_fields->m_particles.empty()) {
            m_fields->m_particles.push_back(night::addParticle(this, "init-big-stars"_spr, "8a0.1a4a2a-1a180a0a40a3a500a200a0a0a0a0a0a0a8a2a45a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a0a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
            m_fields->m_particles.push_back(night::addParticle(this, "big-stars"_spr, "30a-1a8a1a3a180a0a40a3a600a200a0a0a0a0a0a0a8a2a45a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a0a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
            m_fields->m_particles.push_back(night::addParticle(this, "init-small-stars"_spr, "120a0.1a4a2a-1a180a0a40a3a500a200a0a0a0a0a0a0a2a1a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
            m_fields->m_particles.push_back(night::addParticle(this, "small-stars"_spr, "260a-1a8a1a28a180a0a40a3a600a200a0a0a0a0a0a0a2a1a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a1a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
            m_fields->m_particles.push_back(night::addParticle(this, "init-space-dust"_spr, "30a0.1a4a2a-1a180a0a52a5a600a200a0a0a0a0a0a0a200a150a0a360a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a0a0a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
            m_fields->m_particles.push_back(night::addParticle(this, "space-dust"_spr, "40a-1a8a1a4a180a0a52a5a600a200a0a0a0a0a0a0a200a150a0a360a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a0a0a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0"));
        }

        m_fields->m_isUpdateColorScheduled = false;
    }

    void initDayMode() {
        auto background = m_backgroundSprite;
        auto ground1 = m_fields->m_ground1;
        auto ground2 = m_fields->m_ground2;
        auto groundLine = m_fields->m_groundLine;

        std::array<CCSprite*, 3> sprites = {background, ground1, ground2};

        for (auto sprite : sprites) {
            if (!sprite) continue;
            sprite->setColor({0, 102, 255});
        }

        if (groundLine) groundLine->setColor({255, 255, 255});

        if (!m_fields->m_isUpdateColorScheduled) {
            m_initCount = 1;
        }
    }

    void cleanupBeforeSwitch() {
        m_backgroundSprite->stopAllActions();
        if (m_fields->m_ground1) m_fields->m_ground1->stopAllActions();
        if (m_fields->m_ground2) m_fields->m_ground2->stopAllActions();
        if (m_fields->m_groundLine) m_fields->m_groundLine->stopAllActions();

        for (auto particleRef : m_fields->m_particles) {
            if (auto particle = particleRef.lock()) particle->setVisible(Mod::get()->getSettingValue<bool>("night-mode"));
        }

        GameManager::get()->playMenuMusic();
    }

    $override
    virtual bool init() {
        if (!MenuGameLayer::init()) return false;

        m_fields->m_bgColor = night::adjustBrightness(ccc3(5, 17, 35));
        auto groundColorHSV = color::rgb2hsv(ccc3(5, 17, 35));
        groundColorHSV.s *= 0.96;
        groundColorHSV.v += 25;
        m_fields->m_groundColor = night::adjustBrightness(color::hsv2rgb(groundColorHSV));
        m_fields->m_lineColor = night::adjustBrightness(ccc3(175, 214, 231));

        m_backgroundSprite->setZOrder(-10);

        if (auto groundSprites = querySelector("ground > ground-sprites")) m_fields->m_ground1 = typeinfo_cast<CCSprite*>(groundSprites->getChildren()->firstObject());
        if (auto groundSprites2 = querySelector("ground > ground-sprites-2")) m_fields->m_ground2 = typeinfo_cast<CCSprite*>(groundSprites2->getChildren()->firstObject());
        m_fields->m_groundLine = typeinfo_cast<CCSprite*>(querySelector("ground > line"));

        if (night::isNightMode()) initNightMode();
        else initDayMode();

        return true;
    }

    $override
    void updateColor(float dt) {
        if (!night::isNightMode()) MenuGameLayer::updateColor(dt);
    }
};


class $modify(NightMenuLayer, MenuLayer) {
    struct Fields {
        CCSprite* m_daySprFill;
        CCSprite* m_nightSpr;
        short m_dayColorIndex = 1;
    };

    void updateDayColor(float dt) {
        auto hsvColor = color::rgb2hsv(m_menuGameLayer->getBGColor(m_fields->m_dayColorIndex));
        hsvColor.s *= 0.2;
        auto color = color::hsv2rgb(hsvColor);
    
        m_fields->m_nightSpr->stopAllActions();

        m_fields->m_daySprFill->runAction(CCTintTo::create(5, color.r, color.g, color.b));

        m_fields->m_dayColorIndex++;
        m_fields->m_dayColorIndex %= 8;
    }

    void toggleNightMode(CCObject* sender) {
        bool isNight = Mod::get()->getSettingValue<bool>("night-mode");
        Mod::get()->setSettingValue("night-mode", !isNight);

        auto mgl = static_cast<NightMenuGameLayer*>(m_menuGameLayer);
        mgl->cleanupBeforeSwitch();

        if (isNight) {
            mgl->initDayMode();
            m_fields->m_daySprFill->stopAllActions();
            auto hsvColor = color::rgb2hsv(m_menuGameLayer->getBGColor(0));
            hsvColor.s *= 0.2;
            m_fields->m_daySprFill->setColor(color::hsv2rgb(hsvColor));
            m_fields->m_dayColorIndex = 1;
        }
        else {
            mgl->initNightMode();

            m_fields->m_nightSpr->setColor(ccc3(93, 123, 169));
            m_fields->m_nightSpr->stopAllActions();
            m_fields->m_nightSpr->runAction(createHueShiftAction(ccc3(93, 123, 169), 1));
        };
    }

    $override
    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSettingValue<bool>("quick-toggle")) return true;
        bool isNight = Mod::get()->getSettingValue<bool>("night-mode");

        auto logo = getChildByID("main-title");
        if (!logo->isVisible()) return true;
        CCPoint topRightLogo = logo->getPosition() + logo->getContentSize() / 2;

        auto nightSpr = CCSprite::createWithSpriteFrameName("night-icon.png"_spr);

        auto daySpr = CCSprite::createWithSpriteFrameName("sun-icon-stroke.png"_spr);
        auto daySprFill = CCSprite::createWithSpriteFrameName("sun-icon-fill.png"_spr);
        daySprFill->setPosition(daySpr->getContentSize() / 2);
        daySprFill->setZOrder(-1);
        daySpr->addChild(daySprFill);

        auto toggleBtn = CCMenuItemToggler::create(daySpr, nightSpr, this, menu_selector(NightMenuLayer::toggleNightMode));
        toggleBtn->setScale(0.65);
        toggleBtn->toggle(isNight);

        auto menu = CCMenu::createWithItem(toggleBtn);
        menu->setPosition(topRightLogo + ccp(2, -6));
        addChild(menu);

        m_fields->m_daySprFill = daySprFill;
        m_fields->m_nightSpr = nightSpr;

        daySpr->runAction(CCRepeatForever::create(CCRotateBy::create(20, 360)));

        daySprFill->setColor(m_menuGameLayer->getBGColor(0));
        updateDayColor(0);
        schedule(schedule_selector(NightMenuLayer::updateDayColor), 5);

        nightSpr->setColor(ccc3(93, 123, 169));
        nightSpr->runAction(createHueShiftAction(ccc3(93, 123, 169), 1));

        return true;
    }
};
#include "../NightManager.hpp"
#include "../misc/Color.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/MenuGameLayer.hpp>
#include <Geode/binding/GameManager.hpp>
using namespace geode::prelude;

CCAction* createHueShiftAction(ccColor3B defaultColor, float factor) {
    auto hueLeft = color::rgb2hsv(defaultColor);
    hueLeft.h -= 15 * factor;
    auto rgbHueLeft = color::hsv2rgb(hueLeft);

    auto hueRight = color::rgb2hsv(defaultColor);
    hueRight.h += 15 * factor;
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
    $override
    virtual bool init() {
        if (!MenuGameLayer::init()) return false;
        if (!night::isNightMode()) return true;

        ccColor3B bgColor = night::adjustBrightness(ccc3(5, 17, 35));
        ccHSVValue groundColorHSV = color::rgb2hsv(ccc3(5, 17, 35));
        groundColorHSV.s *= 0.95;
        groundColorHSV.v += 0.15;
        ccColor3B groundColor = night::adjustBrightness(color::hsv2rgb(groundColorHSV));
        ccColor3B lineColor = night::adjustBrightness(ccc3(175, 214, 231));

        CCSprite* ground1 = nullptr;
        CCSprite* ground2 = nullptr;

        auto background = m_backgroundSprite;
        if (auto groundSprites = querySelector("ground > ground-sprites")) ground1 = typeinfo_cast<CCSprite*>(groundSprites->getChildren()->firstObject());
        if (auto groundSprites2 = querySelector("ground > ground-sprites-2")) ground2 = typeinfo_cast<CCSprite*>(groundSprites2->getChildren()->firstObject());
        auto groundLine = typeinfo_cast<CCSprite*>(querySelector("ground > line"));

        background->setZOrder(-10);

        background->setColor(bgColor);
        background->runAction(createHueShiftAction(bgColor, 1));
        if (ground1) {
            setGroundColor(ground1, groundColor);
            ground1->runAction(createHueShiftAction(groundColor, 0.8));
        }
        if (ground2) {
            setGroundColor(ground2, groundColor);
            ground2->runAction(createHueShiftAction(groundColor, 0.8));
        }
        if (groundLine) {
            groundLine->setColor(lineColor);
            groundLine->runAction(createHueShiftAction(lineColor, 0.8));
        }

        if (Mod::get()->getSettingValue<std::string>("background-stars") == "Show None") return true;

        night::addParticle(this, "init-big-stars"_spr, "8a0.1a4a2a-1a180a0a40a3a500a200a0a0a0a0a0a0a8a2a45a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a0a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0");
        night::addParticle(this, "big-stars"_spr, "30a-1a8a1a3a180a0a40a3a600a200a0a0a0a0a0a0a8a2a45a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a0a0a0a0a0.376471a0.1a0.52549a0.1a0.662745a0.1a0.8a0.3a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a169a0a0a0a0a0a0a1a0a0a0a0a0a0a0");
        night::addParticle(this, "init-small-stars"_spr, "120a0.1a4a2a-1a180a0a40a3a500a200a0a0a0a0a0a0a2a1a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0");
        night::addParticle(this, "small-stars"_spr, "260a-1a8a1a28a180a0a40a3a600a200a0a0a0a0a0a0a2a1a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a0a0a0a0a0.192157a0.1a0.392157a0.1a0.576471a0.1a0.6a0.4a1a0a2a0a0a0a0a0a0a0a0a2a1a0a0a0a28a0a0a0a0a0a0a1a0a0a0a0a0a0a0");
        night::addParticle(this, "init-space-dust"_spr, "30a0.1a4a2a-1a180a0a52a5a600a200a0a0a0a0a0a0a200a150a0a360a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a0a0a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0");
        night::addParticle(this, "space-dust"_spr, "40a-1a8a1a4a180a0a52a5a600a200a0a0a0a0a0a0a200a150a0a360a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a0a0a0a0a0.00784314a0.01a0.0431373a0.01a0.121569a0.01a0.17a0.1a1a0a2a0a0a0a0a0a0a0a0a2a1a1a0a0a46a0a0a0a0a0a0a1a0a0a0a0a0a0a0");

        return true;
    }

    $override
    void updateColor(float dt) {
        if (!night::isNightMode()) MenuGameLayer::updateColor(dt);
    }

    void setGroundColor(CCSprite* ground, ccColor3B color) {
        ground->setColor(color);

        // this may seem useless, but removing this causes the ground to flash
        // the old blue color for a single frame on startup

        CCArrayExt<CCSprite*> groundSprites = ground->getChildren();
        for (auto groundSprite : groundSprites) {
            groundSprite->setColor(color);
        }
    }
};


class $modify(NightMenuLayer, MenuLayer) {
    struct Fields {
        CCMenuItemSpriteExtra* m_toggleBtn;
        CCSprite* m_daySprFill;
        CCSprite* m_nightSpr;
    };

    $override
    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSettingValue<bool>("quick-toggle")) return true;
        bool isNight = Mod::get()->getSettingValue<bool>("night-mode");

        auto logo = getChildByID("main-title");
        if (!logo->isVisible()) return true;
        CCPoint topRightLogo = logo->getPosition() + logo->getContentSize() / 2;

        auto nightSpr = CCSprite::createWithSpriteFrameName("night-icon.png"_spr);
        nightSpr->setScale(0.65);

        auto daySpr = CCSprite::createWithSpriteFrameName("sun-icon-stroke.png"_spr);
        daySpr->setScale(0.65);
        auto daySprFill = CCSprite::createWithSpriteFrameName("sun-icon-fill.png"_spr);
        daySprFill->setPosition(daySpr->getContentSize() / 2);
        daySprFill->setZOrder(-1);
        daySpr->addChild(daySprFill);

        auto toggleSpr = isNight ? nightSpr : daySpr;
        auto toggleBtn = CCMenuItemSpriteExtra::create(toggleSpr, this, menu_selector(NightMenuLayer::toggleNightMode));
        toggleBtn->setID("quick-toggle-btn"_spr);

        auto menu = CCMenu::createWithItem(toggleBtn);
        menu->setPosition(topRightLogo + ccp(2, -6));
        menu->setID("quick-toggle-menu"_spr);
        addChild(menu);

        m_fields->m_toggleBtn = toggleBtn;
        m_fields->m_daySprFill = daySprFill;
        m_fields->m_nightSpr = nightSpr;

        daySpr->runAction(CCRepeatForever::create(CCRotateBy::create(20, 360)));

        updateDayNightSprites(0);
        schedule(schedule_selector(NightMenuLayer::updateDayNightSprites));

        // weird color changes and occasional crashes can happen when spamming the toggle button too quickly
        // hacky fix but like why would you want to spam it anyway

        toggleBtn->setEnabled(false);
        scheduleOnce(schedule_selector(NightMenuLayer::enableToggleBtn), 0.15);

        return true;
    }

    void enableToggleBtn(float dt) {
        if (auto spr = m_fields->m_toggleBtn) spr->setEnabled(true);
    }

    void toggleNightMode(CCObject* sender) {
        bool isNight = Mod::get()->getSettingValue<bool>("night-mode");
        Mod::get()->setSettingValue("night-mode", !isNight);

        auto menuLayer = MenuLayer::scene(false);
        auto transitionFade = CCTransitionFade::create(0.5, menuLayer);
        CCDirector::sharedDirector()->replaceScene(transitionFade);
    }

    void updateDayNightSprites(float dt) {
        auto bgColor = m_menuGameLayer->m_backgroundSprite->getColor();
        auto bgHSV = color::rgb2hsv(bgColor);

        if (night::isNightMode()) {
            bgHSV.h -= 5;
            bgHSV.s *= 0.8;
            bgHSV.v += 0.25;

            if (auto spr = m_fields->m_nightSpr) spr->setColor(color::hsv2rgb(bgHSV));
        } else {
            bgHSV.s *= 0.3;
            bgHSV.v = 1;

            if (auto spr = m_fields->m_daySprFill) spr->setColor(color::hsv2rgb(bgHSV));
        }
    }
};
#include "../NightManager.hpp"
#include <Geode/modify/CCScale9Sprite.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
using namespace geode::prelude;

class $modify(FixedCCScale9Sprite, CCScale9Sprite) {
    void fixSprite() {
        // this fix and happy textures work fine together, but i'm just assuming the implementation in
        // happy textures is more compatible with texture packs than mine.

        if (Loader::get()->isModLoaded("alphalaneous.happy_textures")) {
            auto ht = Loader::get()->getLoadedMod("alphalaneous.happy_textures");
            if (ht->getSettingValue<bool>("ccscale9sprite-fix")) return;
        }

        CCSprite* sprites[3][3] = {
            {_topLeft, _top, _topRight},
            {_left, _centre, _right},
            {_bottomLeft, _bottom, _bottomRight}
        };

        float outer = m_spriteRect.size.height * 0.15;
        float center = m_spriteRect.size.height * 0.7;

        float positions[3] = {0, outer, outer + center};
        float sizes[3] = {outer, center, outer};

        for (short row = 0; row < 3; row++) {
            for (short col = 0; col < 3; col++) {
                auto sprite = sprites[row][col];
                sprite->setTextureRect({positions[col], positions[row], sizes[col], sizes[row]});
            }
        }
    }

    void toColoredSprite() {
        auto texture = CCSprite::create("square02b_001.png")->getTexture();
        _scale9Image->setTexture(texture);
    }

    void additiveBlending() {
        _scale9Image->setBlendFunc({GL_ONE, GL_ONE_MINUS_CONSTANT_ALPHA});
    }
};

class $modify(EditLevelLayer) {
    $override
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;
        if (!night::isNightMode()) return true;
        if (!Mod::get()->getSettingValue<bool>("recolor-scale9sprite")) return true;

        for (auto child : CCArrayExt<CCNode*>(getChildren())) {
            if (auto sprite = typeinfo_cast<CCScale9Sprite*>(child)) {
                sprite->setColor(night::adjustBrightness(ccc3(30, 56, 95)));
                sprite->setOpacity(60);

                static_cast<FixedCCScale9Sprite*>(sprite)->fixSprite();
                static_cast<FixedCCScale9Sprite*>(sprite)->additiveBlending();
            }
        }

        return true;
    }
};

class $modify(LevelSearchLayer) {
    $override
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;
        if (!night::isNightMode()) return true;
        if (!Mod::get()->getSettingValue<bool>("recolor-scale9sprite")) return true;

        for (auto child : CCArrayExt<CCNode*>(getChildren())) {
            if (auto sprite = typeinfo_cast<CCScale9Sprite*>(child)) {
                if (sprite->getID() != "level-search-bar-bg") {
                    sprite->setColor(night::adjustBrightness(ccc3(10, 28, 55)));
                    sprite->setOpacity(60);

                    static_cast<FixedCCScale9Sprite*>(sprite)->additiveBlending();
                } else {
                    sprite->setColor(ccc3(0, 0, 0));
                    sprite->setOpacity(60);
                }

                static_cast<FixedCCScale9Sprite*>(sprite)->fixSprite();
            }
        }

        return true;
    }
};

class $modify(GJGarageLayer) {
    $override
    virtual bool init() {
        if (!GJGarageLayer::init()) return false;
        if (!night::isNightMode()) return true;
        if (!Mod::get()->getSettingValue<bool>("recolor-scale9sprite")) return true;

        if (auto sprite = typeinfo_cast<CCScale9Sprite*>(getChildByID("select-background"))) {
            sprite->setColor(night::adjustBrightness(ccc3(35, 35, 35)));
            sprite->setOpacity(60);

            static_cast<FixedCCScale9Sprite*>(sprite)->toColoredSprite();
            static_cast<FixedCCScale9Sprite*>(sprite)->additiveBlending();
        }

        return true;
    }
};

#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;

// a lot of this code is copied from geode's implementation of FloatSettingV3
// i wish it was easier to make slight modifications to existing settings

class AdjustBrightnessSettingV3 : public SettingBaseValueV3<float> {
public:
    float m_minValue;
    float m_maxValue;
    float m_sliderSnap;

    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key, std::string const& modID, matjson::Value const& json);

    SettingNodeV3* createNode(float width) override;
};

class AdjustBrightnessNodeV3 : public SettingValueNodeV3<AdjustBrightnessSettingV3> {
protected:
    CCLabelBMFont* m_valueLabel;
    Slider* m_slider;
    CCSprite* m_colorSprite;

    bool init(std::shared_ptr<AdjustBrightnessSettingV3> setting, float width);

    void updateState(CCNode* invoker) override;
    float valueFromSlider(float num);
    void onSlider(CCObject*);

public:
    static AdjustBrightnessNodeV3* create(std::shared_ptr<AdjustBrightnessSettingV3> setting, float width) {
        auto ret = new AdjustBrightnessNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

template <>
struct geode::SettingTypeForValueType<float> {
    using SettingType = AdjustBrightnessSettingV3;
};

$execute {
    (void)Mod::get()->registerCustomSettingType("adjust-brightness", &AdjustBrightnessSettingV3::parse);
}

#include "BrightnessSetting.hpp"
#include "../NightManager.hpp"
#include <Geode/loader/SettingV3.hpp>

Result<std::shared_ptr<AdjustBrightnessSettingV3>> AdjustBrightnessSettingV3::parse(
    std::string const& key,
    std::string const& modID,
    matjson::Value const& json
) {
    auto res = std::make_shared<AdjustBrightnessSettingV3>();
    auto root = checkJson(json, "AdjustBrightnessSettingV3");

    res->parseBaseProperties(key, modID, root);
    root.has("min").into(res->m_minValue);
    root.has("max").into(res->m_maxValue);
    root.has("slider-step").into(res->m_sliderSnap);

    root.checkUnknownKeys();

    return root.ok(res);
}

SettingNodeV3* AdjustBrightnessSettingV3::createNode(float width) {
    return AdjustBrightnessNodeV3::create(
        std::static_pointer_cast<AdjustBrightnessSettingV3>(shared_from_this()),
        width
    );
}

bool AdjustBrightnessNodeV3::init(std::shared_ptr<AdjustBrightnessSettingV3> setting, float width) {
    if (!SettingValueNodeV3<AdjustBrightnessSettingV3>::init(setting, width))
        return false;

    auto topMenu = CCMenu::create();
    topMenu->setLayout(
        RowLayout::create()
            ->setGap(10)
    );
    topMenu->setScale(.5f);
    getButtonMenu()->addChildAtPosition(topMenu, Anchor::Center);

    m_colorSprite = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    m_colorSprite->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setRelativeScale(1.1f)
    );
    topMenu->addChild(m_colorSprite);

    m_valueLabel = CCLabelBMFont::create("1.00x", "bigFont.fnt");
    topMenu->addChild(m_valueLabel);

    setContentHeight(45);
    getButtonMenu()->updateAnchoredPosition(Anchor::Right, ccp(-10, 7));

    m_slider = Slider::create(this, menu_selector(AdjustBrightnessNodeV3::onSlider));
    m_slider->setScale(.5f);
    getButtonMenu()->addChildAtPosition(m_slider, Anchor::Center, ccp(0, -17), ccp(0, 0));

    topMenu->updateLayout();
    setValue(setting->getValue(), nullptr);
    m_slider->setValue((getValue() - setting->m_minValue) / (setting->m_maxValue - setting->m_minValue));
    updateState(nullptr);

    return true;
}

void AdjustBrightnessNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3<AdjustBrightnessSettingV3>::updateState(invoker);
    auto enable = getSetting()->shouldEnable();

    auto str = numToString(getValue(), 2) + "x";
    m_valueLabel->setString(str.c_str());

    m_colorSprite->setColor(ccc3(5 * getValue(), 17 * getValue(), 35 * getValue()));

    m_slider->updateBar();
    m_slider->m_sliderBar->setColor(enable ? ccWHITE : ccGRAY);
    m_slider->m_touchLogic->m_thumb->setColor(enable ? ccWHITE : ccGRAY);
    m_slider->m_touchLogic->m_thumb->setEnabled(enable);
}

float AdjustBrightnessNodeV3::valueFromSlider(float num) {
    auto min = getSetting()->m_minValue;
    auto max = getSetting()->m_maxValue;
    auto range = max - min;
    auto value = num * range + min;
    auto step = getSetting()->m_sliderSnap;

    value = round(value / step) * step;
    if (value > 0.93 && value < 1.07) value = 1;

    m_slider->setValue((value - min) / range);

    return value;
}

void AdjustBrightnessNodeV3::onSlider(CCObject*) {
    setValue(valueFromSlider(m_slider->m_touchLogic->m_thumb->getValue()), m_slider);
}

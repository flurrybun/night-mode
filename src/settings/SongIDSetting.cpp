#include "SongIDSetting.hpp"
#include <Geode/loader/SettingV3.hpp>

Result<std::shared_ptr<SettingV3>> SongIDSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto res = std::make_shared<SongIDSettingV3>();
    auto root = checkJson(json, "SongIDSettingV3");

    res->parseBaseProperties(key, modID, root);
    root.has("placeholder").into(res->m_placeholder);

    root.checkUnknownKeys();

    return root.ok(std::static_pointer_cast<SettingV3>(res));
}

SettingNodeV3* SongIDSettingV3::createNode(float width) {
    return SongIDNodeV3::create(
        std::static_pointer_cast<SongIDSettingV3>(shared_from_this()),
        width
    );
}

bool SongIDNodeV3::init(std::shared_ptr<SongIDSettingV3> setting, float width) {
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    m_toggleBtn = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(SongIDNodeV3::onCheckbox), 1
    );
    m_toggleBtn->toggle(getValue().first);
    m_toggleBtn->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setRelativeScale(0.55f)
    );

    m_input = TextInput::create(110, "Text");
    m_input->setEnabled(getValue().first);
    m_input->setCallback([this](auto const& str) {
        setValue({getValue().first, safeStringToInt(str)}, m_input);
    });
    m_input->setFilter("0123456789");
    m_input->setMaxCharCount(9);
    m_input->getInputNode()->m_numberInput = true;
    m_input->setPlaceholder(setting->m_placeholder);
    m_input->setString(getValue().second != 0 ? std::to_string(getValue().second) : "");
    m_input->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setRelativeScale(0.67f)
    );

    getButtonMenu()->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(5)
    );
    getButtonMenu()->addChild(m_toggleBtn);
    getButtonMenu()->addChild(m_input);
    getButtonMenu()->updateLayout();

    updateState(nullptr);

    return true;
}

void SongIDNodeV3::onCheckbox(CCObject* sender) {
    setValue({!m_toggleBtn->isOn(), getValue().second}, m_toggleBtn);
}

void SongIDNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3::updateState(invoker);
    auto enable = getSetting()->shouldEnable();

    getButtonMenu()->setTouchEnabled(enable);
    m_toggleBtn->m_offButton->setOpacity(enable ? 255 : 100);
    m_toggleBtn->m_onButton->setOpacity(enable ? 255 : 100);
    m_input->setEnabled(enable && getValue().first);

    m_input->setString(getValue().second != 0 ? std::to_string(getValue().second) : "");
}

int SongIDNodeV3::safeStringToInt(const std::string& str) {
    errno = 0;
    char* end;
    long value = std::strtol(str.c_str(), &end, 10);

    if (end == str.c_str() || *end != '\0' || errno != 0 || value <= 0) {
        return 0;
    }

    return static_cast<int>(value);
}

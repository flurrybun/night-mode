#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/Geode.hpp>
using namespace geode::prelude;

// a lot of this code is copied from geode's implementation of StringSettingV3

using SongID = std::pair<bool, int>;

template <>
struct matjson::Serialize<SongID> {
    static matjson::Value toJson(SongID const& value) {
        return matjson::makeObject({
            {"enabled", value.first},
            {"id", value.second}
        });
    }
    static Result<SongID> fromJson(matjson::Value const& value) {
        GEODE_UNWRAP_INTO(bool enabled, value["enabled"].asBool());
        GEODE_UNWRAP_INTO(int id, value["id"].asInt());

        return Ok(SongID(enabled, id));
    }
    static bool isJson(matjson::Value const& json) {
        if (!json.isObject()) return false;
        if (!json.contains("enabled") || !json.contains("id")) return false;
        return json["enabled"].isBool() && json["id"].isNumber();
    }
};

class SongIDSettingV3 : public SettingBaseValueV3<SongID> {
public:
    std::string m_placeholder;

    static Result<std::shared_ptr<SettingV3>> parse(
        std::string const& key, std::string const& modID, matjson::Value const& json);

    SettingNodeV3* createNode(float width) override;
};

class SongIDNodeV3 : public SettingValueNodeV3<SongIDSettingV3> {
protected:
    TextInput* m_input;
    CCMenuItemToggler* m_toggleBtn;

    bool init(std::shared_ptr<SongIDSettingV3> setting, float width);

    void onCheckbox(CCObject* sender);
    void updateState(CCNode* invoker) override;
    int safeStringToInt(std::string const& str);

public:
    static SongIDNodeV3* create(std::shared_ptr<SongIDSettingV3> setting, float width) {
        auto ret = new SongIDNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

template <>
struct geode::SettingTypeForValueType<SongID> {
    using SettingType = SongIDSettingV3;
};

$execute {
    (void)Mod::get()->registerCustomSettingType("song-id", &SongIDSettingV3::parse);
}

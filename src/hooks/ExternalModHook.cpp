#include "../NightManager.hpp"
#include "../misc/Color.hpp"
#include <Geode/modify/CCDirector.hpp>
using namespace geode::prelude;

#ifndef GEODE_IS_WINDOWS
#include <cxxabi.h>
#endif

// there is always a possibility of two mods using the same layer name, so i'm
// also checking if the mod is loaded and if the bg matches an exact query selector

// collisions should be extremely rare, if not nonexistent. but if it does happen,
// it'll only be a minor cosmetic bug and shouldn't cause any major issues.
// if it does happen, i'll have to implement a system to verify the layer through other means
// like checking for nodes besides the background or something

static std::map<std::string, std::vector<night::LayerInfo>> layerInfo = {
    {"dankmeme.globed2", {
        {"GlobedServersLayer", "> background", night::BGColor::Gray},
        {"GlobedMenuLayer", "> background"},
        {"GlobedLevelListLayer", "> background"},
        {"GlobedSettingsLayer", "> background"},
    }},
    {"cvolton.betterinfo", {
        {"CustomCreatorLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
        {"LevelSearchViewLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
        {"DailyViewLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
        {"RewardGroupLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
        {"RewardViewLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
        {"LeaderboardViewLayer", "> cvolton.betterinfo/background", night::BGColor::Purple},
    }},
    {"geode.texture-loader", {
        {"PackSelectLayer", "> background"},
    }},
    {"km7dev.gdps-switcher", {
        {"ServerSwitchLayer", "> background"},
    }},
    {"minemaker0430.gddp_integration", {
        {"DPLayer", "> bg"},
    }},
    {"omgrod.garage_plus", {
        {"GPFeedbackLayer", "> menu > background"},
        {"GPKofiLayer", "> background"},
    }},
    // {"gdutilsdevs.gdutils", {
    //     {"MoreLeaderboards", "> background"},
    // }},
};

// this function was ripped straight from devtools and i have no shame

std::string getNodeName(CCObject* node) {
#ifdef GEODE_IS_WINDOWS
    return typeid(*node).name() + 6;
#else 
    {
        std::string ret;

        int status = 0;
        auto demangle = abi::__cxa_demangle(typeid(*node).name(), 0, 0, &status);
        if (status == 0) {
            ret = demangle;
        }
        free(demangle);

        return ret;
    }
#endif
}

// static ccColor3B geodeBGColors[] = {
//     ccc3(50, 39, 26),
//     ccc3(52, 27, 20),
//     ccc3(42, 16, 23),
//     ccc3(38, 15, 31),
//     ccc3(29, 12, 32),
//     ccc3(17, 11, 25)
// };

static ccColor3B geodeBGColors[] = {
    ccc3(50, 36, 20),
    ccc3(52, 20, 11),
    ccc3(42, 9, 18),
    ccc3(38, 9, 29),
    ccc3(29, 7, 32),
    ccc3(15, 7, 25)
};

class $modify(CCDirector) {
    void modifyGeodeBackground(CCLayer* layer) {
        // i absolutely do not want this mod breaking the geode mod layer, so i'm making extra sure
        // that every node is in place like the good boy i am before i start messing with it

        if (!layer) return;

        auto container = layer->getChildByID("SwelvyBG");
        if (!container) return;

        container->setZOrder(-10);

        std::array<CCSprite*, 6> sprites;

        for (int i = 0; i < 6; i++) {
            auto sprite = typeinfo_cast<CCSprite*>(container->getChildByID("layer-" + std::to_string(i)));

            if (sprite) sprites[i] = sprite;
            else return;
        }
        
        for (int i = 0; i < 6; i++) {
            sprites[i]->setColor(night::adjustBrightness(geodeBGColors[i]));
        }
    }

    void willSwitchToScene(CCScene* scene) {
        CCDirector::willSwitchToScene(scene);
        if (!night::isNightMode()) return;
        if (!Mod::get()->getSettingValue<bool>("change-other-mods")) return;

        auto layer = scene->getChildByType<CCLayer>(0);
        if (!layer) return;

        auto layerName = getNodeName(layer);

        if (layerName == "ModsLayer") {
            modifyGeodeBackground(layer);
            night::addGeodeParticles(layer);
            return;
        }

        for (auto& [modID, layers] : layerInfo) {
            if (!Loader::get()->isModLoaded(modID)) continue;

            for (auto& layerInfo : layers) {
                if (layerName != layerInfo.layerName) continue;
                night::updateExternalModBackground(layer, modID, layerInfo);
            }
        }
    }
};

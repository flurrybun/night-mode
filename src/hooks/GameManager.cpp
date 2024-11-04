#include "../misc/SongIDSetting.hpp"
#include <Geode/modify/GameManager.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
using namespace geode::prelude;

class $modify(GameManager) {
    $override
    gd::string getMenuMusicFile() {
        if (!Mod::get()->getSettingValue<bool>("night-mode")) return GameManager::getMenuMusicFile();
        if (!Mod::get()->getSettingValue<bool>("change-music")) return GameManager::getMenuMusicFile();
        
        auto songID = Mod::get()->getSettingValue<SongID>("custom-song-id");

        if (songID.first) {
            return MusicDownloadManager::sharedState()->pathForSong(songID.second);
        } else {
            auto path = Mod::get()->getResourcesDir() / "night-menuLoop.mp3";
            return path.string();
        }
    }
};

$execute {
    listenForSettingChanges("custom-song-id", [](SongID value) {
        GameManager::get()->fadeInMenuMusic();
    });

    listenForSettingChanges("change-music", [](bool value) {
        GameManager::get()->fadeInMenuMusic();
    });
}
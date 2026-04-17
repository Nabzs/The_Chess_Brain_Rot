#include "sound_manager.hpp"
#include <unordered_map>
#include <memory>
#include <iostream>

static std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> buffers;
static std::vector<sf::Sound> activeSounds;

static void cleanFinished()
{
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        activeSounds.end());
}

static bool loadOne(const std::string& key, const std::string& path)
{
    auto buf = std::make_unique<sf::SoundBuffer>();
    if (!buf->loadFromFile(path))
    {
        std::cerr << "Son introuvable (optionnel): " << path << std::endl;
        return false;
    }
    buffers[key] = std::move(buf);
    return true;
}

void loadAllSounds(const std::string& assetsDir)
{
    std::string dir = assetsDir + "/sounds/";

    // Sons par type de piece
    loadOne("K",  dir + "king.ogg");
    loadOne("Q",  dir + "queen.ogg");
    loadOne("R",  dir + "tower.ogg");
    loadOne("B",  dir + "bishop.ogg");
    loadOne("Kn", dir + "knight.ogg");
    loadOne("P",  dir + "pion.ogg");

    // Sons d'evenements
    loadOne("explosion",  dir + "explosion.ogg");
    loadOne("twin_tower", dir + "twin_tower.ogg");
    loadOne("fatigue",    dir + "fatigue.ogg");
    loadOne("panic",      dir + "panic.ogg");
    loadOne("expand",     dir + "expand.ogg");
    loadOne("promotion",  dir + "promotion.ogg");
    loadOne("victory",    dir + "victory.ogg");
    loadOne("capture",    dir + "capture.ogg");

    std::cout << "Sons charges: " << buffers.size() << std::endl;
}

void playMoveSound(const std::string& pieceType)
{
    cleanFinished();
    auto it = buffers.find(pieceType);
    if (it == buffers.end())
        return;
    activeSounds.emplace_back(*it->second);
    activeSounds.back().play();
}

void playEventSound(const std::string& eventName)
{
    cleanFinished();
    auto it = buffers.find(eventName);
    if (it == buffers.end())
        return;
    activeSounds.emplace_back(*it->second);
    activeSounds.back().play();
}

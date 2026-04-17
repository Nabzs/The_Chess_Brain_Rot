#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>

void loadAllSounds(const std::string& assetsDir);
void playMoveSound(const std::string& pieceType);
void playEventSound(const std::string& eventName);

#endif // SOUND_MANAGER_HPP

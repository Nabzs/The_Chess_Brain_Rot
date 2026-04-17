#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <string>

void loadAllTextures(const std::string& assetsDir);
const sf::Texture& getTexture(const std::string& pieceType, bool isWhite);

#endif // TEXTURE_MANAGER_HPP

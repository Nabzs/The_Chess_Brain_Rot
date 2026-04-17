#include "texture_manager.hpp"
#include <unordered_map>
#include <memory>
#include <iostream>
#include <stdexcept>

static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;

static std::string typeToName(const std::string& type)
{
    if (type == "K")  return "king";
    if (type == "Q")  return "queen";
    if (type == "R")  return "tower";
    if (type == "B")  return "bishop";
    if (type == "Kn") return "knight";
    if (type == "P")  return "pion";
    return "";
}

static std::string makeKey(const std::string& pieceType, bool isWhite)
{
    return (isWhite ? "white_" : "black_") + typeToName(pieceType);
}

void loadAllTextures(const std::string& assetsDir)
{
    const std::string colors[] = {"white", "black"};
    const std::string names[]  = {"king", "queen", "tower", "bishop", "knight", "pion"};

    for (const auto& color : colors)
    {
        for (const auto& name : names)
        {
            std::string key  = color + "_" + name;
            std::string path = assetsDir + "/" + key + ".png";

            auto tex = std::make_unique<sf::Texture>();
            if (!tex->loadFromFile(path))
            {
                std::cerr << "Failed to load texture: " << path << std::endl;
                continue;
            }
            tex->setSmooth(true);
            textures[key] = std::move(tex);
        }
    }
    std::cout << "Loaded " << textures.size() << " textures." << std::endl;
}

const sf::Texture& getTexture(const std::string& pieceType, bool isWhite)
{
    std::string key = makeKey(pieceType, isWhite);
    auto it = textures.find(key);
    if (it == textures.end())
        throw std::runtime_error("Texture not found: " + key);
    return *it->second;
}

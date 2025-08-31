#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <optional>
#include <stdexcept>

#include <nlohmann_json.hpp>
using json = nlohmann::json;

struct SpriteSheetInfo {
    std::string path;
    int width;
    int height;
    int rows;
    int columns;
    int sprite_width;
    int sprite_height;
    float default_animation_speed;
};

class AssetManager {
public:
    static AssetManager& Get() {
        static AssetManager instance;
        return instance;
    }

    std::optional<SpriteSheetInfo> LoadSpriteSheet(const std::string& filepath, const std::string& key)
     {
        auto it = spriteSheets.find(key);
        if (it != spriteSheets.end()) {
            return it->second;
        }

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Failed to open file '" << filepath << "'\n";
            return std::nullopt;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        json assetData;
        try {
            assetData = json::parse(buffer.str());
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in '" << filepath << "': " << e.what() << std::endl;
            return std::nullopt;
        }

        if (!assetData.contains(key)) {
            std::cerr << "JSON does not contain key '" << key << "' in file '" << filepath << "'\n";
            return std::nullopt;
        }

        const auto& sheet = assetData.at(key);

        try {
            SpriteSheetInfo info {
                sheet.at("path").get<std::string>(),
                sheet.at("width").get<int>(),
                sheet.at("height").get<int>(),
                sheet.at("rows").get<int>(),
                sheet.at("columns").get<int>(),
                sheet.at("sprite_width").get<int>(),
                sheet.at("sprite_height").get<int>(),
                sheet.value("default_animation_speed", 0.1f)
            };

            spriteSheets[key] = info;
            return info;

        } catch (const json::exception& e) {
            std::cerr << "Error reading sprite sheet '" << key << "' data: " << e.what() << std::endl;
            return std::nullopt;
        }
    }

private:
    AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator=(AssetManager&&) = delete;

private:
    std::unordered_map<std::string, SpriteSheetInfo> spriteSheets;
};

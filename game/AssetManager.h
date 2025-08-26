#include <unordered_map>
#include <string>
#include <stdexcept>
#include <fstream>

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
};

class AssetManager {
public:
    static AssetManager& Get() {
        static AssetManager instance;
        return instance;
    }

    const SpriteSheetInfo& LoadSpriteSheet(const std::string& filepath, const std::string& key) {
        auto it = spriteSheets.find(key);
        if (it != spriteSheets.end()) {
            return it->second;
        }

        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open " + filepath);
        }

        json assetData;
        file >> assetData;
        auto& sheet = assetData.at(key);

        SpriteSheetInfo info {
            sheet.at("path").get<std::string>(),
            sheet.at("width").get<int>(),
            sheet.at("height").get<int>(),
            sheet.at("rows").get<int>(),
            sheet.at("columns").get<int>(),
            sheet.at("sprite_width").get<int>(),
            sheet.at("sprite_height").get<int>()
        };

        spriteSheets[key] = info;
        return spriteSheets[key];
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

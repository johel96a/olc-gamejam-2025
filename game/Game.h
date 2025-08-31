#pragma once

// STD Headers

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <regex>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

// External Headers

#include "olcPixelGameEngine.h"
#include "olcPGEX_SplashScreen.h"
#include "olcSoundWaveEngine.h"

// Project Headers

#include "Player.h"

enum class GameState {
    ShapeSelect,
    Ready,
    Playing
};

enum class GameSubState
{
    Countdown,
    Playing,
    Finished
};

struct ShapeData {
    int id = -1;
    std::unordered_set<olc::vi2d> edgePixels;
    bool unlocked = false;
    float accuracy = 0.0f;
};

class Game : public olc::PixelGameEngine {
public:
    Game();
    virtual ~Game();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

private:
    void LoadAllShapes();
    void LoadShape(const std::string& fileName);
    float CheckAccuracy(const std::unordered_set<olc::vi2d>& edgePixels);
    void DrawCenteredString(int y, const std::string& text, olc::Pixel color = olc::WHITE, uint32_t scale = 1);
    void DrawAccuracy(float accuracy);

    // Game State Handlers
    void UpdateShapeSelect();
    void RenderShapeSelect();

    void UpdatePlaying(float fElapsedTime);
    void RenderPlaying();

private:
    //olc::SplashScreen m_olcSplashScreen;
    GameState m_currentGameState;
    GameSubState m_playSubState;

    float m_countdownTimer;
    float m_gameplayTimer;

    std::unique_ptr<Player> m_player;

    std::unique_ptr<olc::Sprite> m_backgroundSprite;
    std::unique_ptr<olc::Sprite> m_shapeSprite;

    int m_currentShapeIndex;
    std::vector<ShapeData> m_shapes;

	olc::sound::WaveEngine m_soundEngine;
	std::unique_ptr<olc::sound::Wave> m_backgroundMusicWave;
    
    bool m_playingDrillSound;
	std::unique_ptr<olc::sound::Wave> m_drillSoundWave;
    olc::sound::PlayingWave m_drillSoundPlayingWave;
};



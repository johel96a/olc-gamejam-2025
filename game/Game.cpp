#include "Game.h"

Game::Game() 
{
    sAppName = "Drill the Shape";
}

Game::~Game() 
{

}

bool Game::OnUserCreate() 
{
    // Load Sprites
    m_backgroundSprite = std::make_unique<olc::Sprite>();
    if (!m_backgroundSprite->LoadFromFile("assets/textures/tileset/background.png")) {
        std::cerr << "Failed to load background.png" << std::endl;
    }

    // Load Sounds
    m_soundEngine.InitialiseAudio(44100, 2);
    m_soundEngine.SetOutputVolume(0.30f);

    m_drillSoundWave = std::make_unique<olc::sound::Wave>();
    m_drillSoundWave->LoadAudioWaveform("assets/audio/drill.wav");
    m_playingDrillSound = false;

    m_backgroundMusicWave = std::make_unique<olc::sound::Wave>();
	m_backgroundMusicWave->LoadAudioWaveform("assets/audio/music.wav");	

    m_soundEngine.PlayWaveform(m_backgroundMusicWave.get(), true);

    // Load All Shapes
    LoadAllShapes();
    m_shapes[0].unlocked = true;

    m_currentGameState = GameState::ShapeSelect;
    m_currentShapeIndex = 0;

    m_player = std::make_unique<Player>();
    m_player->SetPosition({88, 48});
    m_player->SetOnDrillCompleteCallback([this](){
        m_gameplayTimer = 0.f;
    });

    return true; 
}

bool Game::OnUserUpdate(float fElapsedTime) 
{ 
    DrawSprite({0, 0}, m_backgroundSprite.get());

    switch (m_currentGameState)
    {
        case GameState::ShapeSelect:
            UpdateShapeSelect();
            RenderShapeSelect();
            break;

        case GameState::Ready:
            m_currentGameState = GameState::Playing;
            m_playSubState = GameSubState::Countdown;
            m_countdownTimer = 3.0f;
            m_gameplayTimer = 60.0f;
            break;

        case GameState::Playing:
            UpdatePlaying(fElapsedTime);
            RenderPlaying();
            break;
    }

    m_player->Render(this);

    return true; 
}

void Game::LoadShape(const std::string& fileName) 
{
    auto shape = std::make_unique<olc::Sprite>();
    if (!shape->LoadFromFile("assets/textures/shapes/" + fileName)) {
        std::cerr << "Failed to load " + fileName +"\n";
    }
    const int width = shape->width;
    const int height = shape->height;

    const olc::vi2d shapeOffset = {
        (ScreenWidth() - width) / 2,
        (ScreenHeight() - height) / 2
    };

    ShapeData shapeData;
    shapeData.edgePixels.reserve(width * height / 4);

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            olc::Pixel current = shape->GetPixel(x, y);
            if (current.a == 0)
                continue;

            bool isEdge = false;
            // Check 8 neighbors for transparency (alpha == 0)
            for (int ny = -1; ny <= 1 && !isEdge; ny++)
            {
                for (int nx = -1; nx <= 1 && !isEdge; nx++)
                {
                    if (nx == 0 && ny == 0) continue;
                    olc::Pixel neighbor = shape->GetPixel(x + nx, y + ny);
                    if (neighbor.a == 0) {
                        isEdge = true;
                    }
                }
            }

            if (isEdge) {       
                shapeData.edgePixels.insert({ x + shapeOffset.x, y + shapeOffset.y });
            }
        }
    } 

    std::string nameWithoutExt = fileName.substr(0, fileName.find_last_of('.'));
    int number = std::stoi(nameWithoutExt);
    shapeData.id = number;

    m_shapes.push_back(std::move(shapeData));
}

void Game::LoadAllShapes() 
{
    std::vector<std::pair<int, std::string>> numberedFiles;

    std::regex pattern(R"((\d+)\.png)"); // match digits followed by .png

    for (const auto& entry : fs::directory_iterator("assets/textures/shapes"))
    {
        if (entry.is_regular_file())
        {
            const auto& path = entry.path();
            if (path.extension() == ".png")
            {
                std::string filename = path.filename().string();

                std::smatch match;
                if (std::regex_match(filename, match, pattern))
                {
                    int number = std::stoi(match[1].str());
                    numberedFiles.emplace_back(number, filename);
                }
            }
        }
    }

    std::sort(numberedFiles.begin(), numberedFiles.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    for (const auto& [number, filename] : numberedFiles)
    {
        LoadShape(filename);
    }
}

float Game::CheckAccuracy(const std::unordered_set<olc::vi2d>& edgePixels) 
{
    int matched = 0;
    const auto& drilled = m_player->GetDrilledPixels();
    std::unordered_set<olc::vi2d> edgeSet(edgePixels.begin(), edgePixels.end());

    for (const auto& edgePixel : edgeSet)
    {
        if (drilled.count(edgePixel) > 0)
        {
            matched++;
        }
    }

    int total = static_cast<int>(edgeSet.size());
    float accuracy = total > 0 ? (matched / static_cast<float>(total)) * 100.0f : 0.0f;
    return accuracy * 0.01f;
}

void Game::UpdateShapeSelect()
{
    if (GetKey(olc::Key::A).bPressed)
    {
        do {
            m_currentShapeIndex = (m_currentShapeIndex - 1 + m_shapes.size()) % m_shapes.size();
        } while (!m_shapes[m_currentShapeIndex].unlocked);
    }

    if (GetKey(olc::Key::D).bPressed)
    {
        do {
            m_currentShapeIndex = (m_currentShapeIndex + 1) % m_shapes.size();
        } while (!m_shapes[m_currentShapeIndex].unlocked);
    }

    if (GetKey(olc::Key::SPACE).bPressed)
    {
        m_currentGameState = GameState::Ready;
    }
}

void Game::RenderShapeSelect()
{
    DrawCenteredString(10, "Select a shape (A / D)", olc::YELLOW, 1);
    DrawCenteredString(25, "Press SPACE to play", olc::YELLOW, 1);
    DrawCenteredString(215, "Get 80% accuracy to unlock new shapes", olc::WHITE, 1);

    if (!m_shapes.empty() && m_shapes[m_currentShapeIndex].unlocked)
    {
        const auto& shapeEdgePixels = m_shapes[m_currentShapeIndex].edgePixels;
        for (const auto& p : shapeEdgePixels)
            Draw(p, olc::Pixel(100, 60, 30));

        DrawCenteredString(190, {"Accuracy Score: " + std::to_string((int)m_shapes[m_currentShapeIndex].accuracy) }, olc::DARK_BLUE, 1);
    }

    m_player->Render(this);
}

void Game::UpdatePlaying(float fElapsedTime)
{
    const auto& shapeEdgePixels = m_shapes[m_currentShapeIndex].edgePixels;

    switch (m_playSubState)
    {
        case GameSubState::Countdown:
        {
            m_countdownTimer -= fElapsedTime;
            if (m_countdownTimer <= 0.0f)
            {
                m_playSubState = GameSubState::Playing;
                m_countdownTimer = 0.0f;
            }
            break;
        }

        case GameSubState::Playing:
        {
            m_gameplayTimer -= fElapsedTime;
            if (m_gameplayTimer <= 0.0f)
            {
                m_gameplayTimer = 0.f;
                m_playSubState = GameSubState::Finished;
            }

            m_player->Update(this, fElapsedTime, shapeEdgePixels);

            if (!m_playingDrillSound)
            {
                m_drillSoundPlayingWave = m_soundEngine.PlayWaveform(m_drillSoundWave.get(), true);
                m_playingDrillSound = true;
            }

            break;
        }

        case GameSubState::Finished:
        {
            if (m_playingDrillSound)
            {
                m_soundEngine.StopWaveform(m_drillSoundPlayingWave);
                m_playingDrillSound = false;
            }

            if (GetKey(olc::Key::SPACE).bPressed)
            {
                float accuracy = CheckAccuracy(shapeEdgePixels) * 100;

                if (accuracy > m_shapes[m_currentShapeIndex].accuracy)
                {
                    m_shapes[m_currentShapeIndex].accuracy = accuracy;
                }

                if(accuracy >= 80.f) 
                {
                    int nextShapeIndex = m_currentShapeIndex + 1;
                    if (nextShapeIndex < m_shapes.size() && !m_shapes[nextShapeIndex].unlocked)
                    {
                        m_shapes[nextShapeIndex].unlocked = true;
                        m_currentShapeIndex = nextShapeIndex;
                    }
                }

                m_currentGameState = GameState::ShapeSelect;
                m_playSubState = GameSubState::Countdown;
                m_countdownTimer = 3.0f;
                m_gameplayTimer = 60.0f;

                m_player->Reset();
                m_player->SetPosition({88, 48});
            }

            break;
        }
    }
}

void Game::RenderPlaying()
{
    const auto& shapeEdgePixels = m_shapes[m_currentShapeIndex].edgePixels;

    switch (m_playSubState)
    {
        case GameSubState::Countdown:
        {
            int secondsLeft = static_cast<int>(std::ceil(m_countdownTimer));
            DrawCenteredString(35, {"Starting in " + std::to_string(secondsLeft)}, olc::YELLOW);

            break;
        }

        case GameSubState::Playing:
        {
            int secondsLeft = static_cast<int>(std::ceil(m_gameplayTimer));
            DrawCenteredString(35, {std::to_string(secondsLeft)}, olc::YELLOW, 1);

            break;
        }

        case GameSubState::Finished:
        {
            float accuracy = CheckAccuracy(shapeEdgePixels);
            DrawAccuracy(accuracy);

            DrawCenteredString(215, "Press SPACE to continue", olc::YELLOW, 1);
            
            break;
        }
    }

    for (const auto& p : shapeEdgePixels)
        Draw(p, olc::Pixel(100, 60, 30));
}

void Game::DrawCenteredString(int y, const std::string& text, olc::Pixel color, uint32_t scale)
{
    int textWidth = static_cast<int>(text.length()) * 8 * scale; // 8 pixels per character
    int x = (ScreenWidth() - textWidth) / 2;
    DrawString({ x, y }, text, color, scale);
}

void Game::DrawAccuracy(float accuracy) 
{ 
    int accuracyPercent = static_cast<int>(accuracy * 100);
    std::string accLabel = "Accuracy: ";
    std::string accVal = std::to_string(accuracyPercent) + "%";
    int screenWidth = ScreenWidth();
    int textWidth = static_cast<int>((accLabel.size() + accVal.size()) * 8);
    int xPos = (screenWidth - textWidth) / 2;
    DrawString({ xPos, 25 }, accLabel, olc::WHITE, 1);
    olc::Pixel color = (accuracyPercent >= 75) ? olc::GREEN : olc::RED;
    int valOffset = accLabel.size() * 8; DrawString({ xPos + valOffset, 25 }, accVal, color, 1); 

}

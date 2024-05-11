#ifndef _ASTEROIDS_H_
#define _ASTEROIDS_H_

#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>

#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

#include "../AppBase.h"
#include "Ship.h"
#include "AsteroidSpawner.h"
#include "EventDispatcher.h"

namespace Asteroids
{
    class Asteroids : public AppBase {
    public:
        Asteroids() : ship(bulletSpawner)
        {
            SetRandomSeed(time(NULL));
            EventDispatcher::AddEventListener(EventType::ASTEROID_DESTROYED, std::bind(&Asteroids::OnAsteroidDestroyed, this, std::placeholders::_1));
            EventDispatcher::AddEventListener(EventType::SHIP_DESTROYED, std::bind(&Asteroids::OnShipDestroyed, this, std::placeholders::_1));
        }

        ~Asteroids()
        {
        }

        void Input() override
        {
            if(gameStarted) {
                ship.Input();
            } else {
                if(IsKeyPressed(KEY_SPACE)) {
                    StartGame();
                }
            }
        }

        void Update() override
        {
            if(gameStarted) {
                if(respawn) {
                    ship.Reset();
                    ship.Invulnerable();
                    respawn = false;
                }

                ship.Update(asteroidSpawner);
                bulletSpawner.Update(asteroidSpawner);
                asteroidSpawner.Update();
                if(asteroidSpawner.Completed())
                {
                    gameStarted = false;
                }
            }
        }

        void Render() override
        {
            ClearBackground(BLACK);
            if(gameStarted) {
                ship.Render();
                asteroidSpawner.Render();
                bulletSpawner.Render();
                std::string scoreText = "Score: " + std::to_string(score);
                DrawText(scoreText.c_str(), 10, 10, 36, WHITE);
                std::string liveText = "Live: " + std::to_string(lives);
                DrawText(liveText.c_str(), 10, 50, 36, WHITE);
            } else if (gameOver) {
                int textWidth = MeasureText("GAME OVER", 42);
                int xPos = (GetScreenWidth() - textWidth) / 2;
                DrawText("GAME OVER", xPos, GetScreenHeight() * 0.2, 42, WHITE);
                
                std::string scoreText = "Score: " + std::to_string(score);
                textWidth = MeasureText(scoreText.c_str(), 36);
                xPos = (GetScreenWidth() - textWidth) / 2;
                DrawText(scoreText.c_str(), xPos, GetScreenHeight() * 0.3, 36, WHITE);

                textWidth = MeasureText("PRESS SPACE TO RESTART", 36);
                xPos = (GetScreenWidth() - textWidth) / 2;
                DrawText("PRESS SPACE TO RESTART", xPos, GetScreenHeight() * 0.5, 36, WHITE);
            } else {
                int textWidth = MeasureText("PRESS SPACE TO START", 36);
                int xPos = (GetScreenWidth() - textWidth) / 2;
                DrawText("PRESS SPACE TO START", xPos, GetScreenHeight() * 0.4, 36, WHITE);
            }
        }

        void OnAsteroidDestroyed(Event& event)
        {
            score++;
        }

        void OnShipDestroyed(Event& event)
        {
            lives--;
            if(lives <= 0) {
                gameStarted = false;
                gameOver = true;
            } else {
                respawn = true;
            }
        }

    private:

        void StartGame()
        {
            gameStarted = true;
            lives = 3;
            score = 0;
            ship.Reset();
            asteroidSpawner.Reset();
            bulletSpawner.Reset();
            NextLevel();
        }

        void NextLevel()
        {
            currentLevel++;
            asteroidSpawner.SpawnLevel();
        }

    private:
        int currentLevel = 0;
        bool gameStarted = false;
        bool gameOver = false;
        bool respawn = false;
        
        int score = 0;
        int lives = 3;

        Ship ship;
        AsteroidSpawner asteroidSpawner;
        BulletSpawner bulletSpawner;
};
} // namespace Asteroids



#endif
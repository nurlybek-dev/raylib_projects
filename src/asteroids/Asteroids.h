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

namespace Asteroids
{
    class Asteroids : public AppBase {
    public:
        Asteroids() : ship(bulletSpawner)
        {
            SetRandomSeed(time(NULL));
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
                    gameStarted = true;
                    NextLevel();
                }
            }
        }

        void Update() override
        {
            if(gameStarted) {
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
            } else {
                int textWidth = MeasureText("PRESS SPACE TO START", 36);
                int xPos = (GetScreenWidth() - textWidth) / 2;
                DrawText("PRESS SPACE TO START", xPos, GetScreenHeight() * 0.4, 36, WHITE);
            }
        }

    private:
        void Restart() 
        {

        }

        void NextLevel()
        {
            currentLevel++;
            asteroidSpawner.SpawnLevel();
        }

    private:
        int currentLevel = 0;
        bool gameStarted = false;
        
        int score = 0;
        int lives = 0;

        Ship ship;
        AsteroidSpawner asteroidSpawner;
        BulletSpawner bulletSpawner;
};
} // namespace Asteroids



#endif
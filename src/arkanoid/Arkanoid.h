#ifndef _ARKANOID_H_
#define _ARKANOID_H_

#include <iostream>
#include <string>
#include <raylib.h>

#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

#include "../AppBase.h"
#include "Ball.h"
#include "Paddle.h"
#include "Level.h"

const int MAX_LEVEL = 3;

namespace Arkanoid
{
    class Arkanoid : public AppBase {
    public:
        Arkanoid()
        {
            level = nullptr;
            NextLevel();
        }

        ~Arkanoid()
        {
            delete level;
            level = nullptr;
        }

        void Input() override
        {
            if(gameCompleted) return;
            paddle.Input();
            if(!ballInGame && IsKeyPressed(KEY_SPACE))
            {
                ballInGame = true;
                ball.SetVelocity({0, -1});
            }
        }

        void Update() override
        {
            if(gameCompleted) return;
            paddle.Update();
            if(!ballInGame) {
                Rectangle paddleBounds = paddle.GetBounds();
                Vector2 ballPosition = {paddleBounds.x + paddleBounds.width / 2, paddleBounds.y - 15};
                ball.SetPosition(ballPosition);
            }
            ball.Update(paddle.GetBounds(), level->GetBricks());
            if(ball.Position().y > GetScreenHeight())
            {
                ballInGame = false;
                ball.SetVelocity({0, 0});
                currentLive--;
            }
            level->Update();
            if(level->IsComplete())
            {
                currentLive++;
                NextLevel();
            }

            if(currentLive <= 0)
            {
                gameOver = true;
            }
        }

        void Render() override
        {
            ClearBackground(BLACK);
            if(gameCompleted || gameOver)
            {
                if(gameCompleted) {
                    int textWidth = MeasureText("You win!", 46);
                    int xPos = (GetScreenWidth() - textWidth) / 2;
                    DrawText("You win!", xPos, GetScreenHeight() * 0.25, 46, WHITE);
                } else if(gameOver) {
                    int textWidth = MeasureText("Game Over.", 46);
                    int xPos = (GetScreenWidth() - textWidth) / 2;
                    DrawText("Game Over.", xPos, GetScreenHeight() * 0.25, 46, WHITE);
                }

                if (GuiButton({GetScreenWidth() / 2.0f - 50, GetScreenHeight() * 0.45f, 100, 20}, "Restart"))
                {
                    Restart();
                }
                if (GuiButton({GetScreenWidth() / 2.0f - 50, GetScreenHeight() * 0.5f, 100, 20}, "Exit"))
                {
                    Close();
                }
            }
            else
            {
                paddle.Render();
                level->Render();
                ball.Render();

                if(!ballInGame)
                {
                    int textWidth = MeasureText("Press SPACE to launch the ball.", 36);
                    int xPos = (GetScreenWidth() - textWidth) / 2;
                    DrawText("Press SPACE to launch the ball.", xPos, GetScreenHeight() / 2, 36, WHITE);
                }

                std::string liveText = "Live: " + std::to_string(currentLive);
                int textWidth = MeasureText(liveText.c_str(), 16);
                int xPos = (GetScreenWidth() * 0.1 - textWidth) / 2;
                DrawText(liveText.c_str(), xPos, GetScreenHeight() * 0.95, 16, WHITE);
            }
        }

        void NextLevel()
        {
            currentLevel++;
            if(currentLevel > MAX_LEVEL)
            {
                gameCompleted = true;
                return;
            }
            paddle.Reset();
            ball.Reset();
            ballInGame = false;
            if(level != nullptr) {
                delete level;
                level = nullptr;
            }
            level = new Level(5 * currentLevel, 5 * currentLevel);
        }

    private:
        void Restart() 
        {
            currentLive = 3;
            currentLevel = 0;
            ballInGame = false;
            gameCompleted = false;
            gameOver = false;
            NextLevel();
        }
    
    private:
        Ball ball;
        Paddle paddle;
        Level* level;
        int currentLive = 3;
        int currentLevel = 0;

        bool ballInGame = false;
        bool gameCompleted = false;
        bool gameOver = false;
};
} // namespace Arkanoid



#endif
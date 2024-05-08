#ifndef _PING_PONG_H_
#define _PING_PONG_H_

#include <iostream>
#include <string>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "../AppBase.h"
#include "Ball.h"
#include "Paddle.h"


namespace PingPong
{
    class PingPong : public AppBase {
        public:
            PingPong()
            {
                gameStarted = false;
                gameEnded = false;
            }
        
            void Input() override
            {

            }

            void Update() override
            {
                if (gameStarted) {
                    if(ball.Position().x <= 0) {
                        playerScore2++;
                        ball.Reset();
                        ball.SetVelocity({0.75, 0.25});
                    } else if (ball.Position().x >= GetScreenWidth()) {
                        playerScore1++;
                        ball.Reset();
                        ball.SetVelocity({-0.75, -0.25});
                    }

                    if(playerScore1 == 10 || playerScore2 == 10) {
                        gameStarted = false;
                        gameEnded = true;
                    }
                }
            }

            void Render() override
            {
                ClearBackground(BLACK);
                Vector2 screenCenter = {GetScreenWidth() / 2, GetScreenHeight() / 2};

                if (gameStarted) {
                    for (int y = 0; y < GetScreenHeight(); ++y)
                    {
                        if (y % 5)
                        {
                            DrawPixel(screenCenter.x, y, WHITE);
                        }
                    }

                    DrawText(std::to_string(playerScore1).c_str(), screenCenter.x - 65, 10, 24, WHITE);
                    DrawText(std::to_string(playerScore2).c_str(), screenCenter.x + 50, 10, 24, WHITE);
                    paddle1.Input(ball.Position());
                    paddle2.Input(ball.Position());
                    
                    ball.Update(paddle1.GetBounds(), paddle2.GetBounds());
                    paddle1.Update();
                    paddle2.Update();

                    ball.Render();
                    paddle1.Render();
                    paddle2.Render();
                } else if(gameEnded) {
                    DrawText(std::to_string(playerScore1).c_str(), screenCenter.x - 65, 10, 24, WHITE);
                    DrawText(std::to_string(playerScore2).c_str(), screenCenter.x + 50, 10, 24, WHITE);
                    if(playerScore1 > playerScore2) {
                        DrawText("Player 1 Win", screenCenter.x - 110, screenCenter.y - 50, 36, WHITE);
                    } else {
                        DrawText("Player 2 Win", screenCenter.x - 110, screenCenter.y - 50, 36, WHITE);
                    }
                    if (GuiButton({screenCenter.x - 50, screenCenter.y + 50, 100, 20}, "Restart"))
                    {
                        Restart();
                    }
                    if (GuiButton({screenCenter.x - 50, screenCenter.y + 80, 100, 20}, "Menu"))
                    {
                        Menu();
                    }
                } else {
                    DrawText("Ping Pong", screenCenter.x - 90, 10, 36, WHITE);
                    if (GuiButton({screenCenter.x - 50, 200, 100, 20}, "1 Player"))
                    {
                        gameStarted = true;
                        paddle1.SetPlayer(1);
                        paddle2.SetAI();
                    }
                    if (GuiButton({screenCenter.x - 50, 230, 100, 20}, "2 Players"))
                    {
                        gameStarted = true;
                        paddle1.SetPlayer(1);
                        paddle2.SetPlayer(2);
                    }
                    if (GuiButton({screenCenter.x - 50, 260, 100, 20}, "Exit"))
                    {
                        Close();
                    }
                }
            }
        
        private:
            void Restart() 
            {
                gameStarted = true;
                gameEnded = false;
                playerScore1 = 0;
                playerScore2 = 0;
                ball.Reset();
                paddle1.Reset();
                paddle2.Reset();
            }

            void Menu()
            {
                gameStarted = false;
                gameEnded = false;
                playerScore1 = 0;
                playerScore2 = 0;
                ball.Reset();
                paddle1.Reset();
                paddle2.Reset();
            }

        private:
            Ball ball;
            Paddle paddle1;
            Paddle paddle2;

            int playerScore1 = 0;
            int playerScore2 = 0;

            bool gameStarted;
            bool gameEnded;
    };    
} // namespace PingPong


#endif
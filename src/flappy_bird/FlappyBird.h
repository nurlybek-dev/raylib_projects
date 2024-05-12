#ifndef _FLAPPY_BIRD_H_
#define _FLAPPY_BIRD_H_

#include <cmath>
#include <string>
#include "raylib.h"

#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

#include "../app.h"
#include "Pipe.h"
#include "Bird.h"

namespace FlappyBird
{
    class FlappyBird : public AppBase
    {
        public:
            FlappyBird()
            {
                center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
                backgroundImage = LoadBackground();
                platformImage = LoadPlatform();

                backgroundPosition = {center.x - backgroundImage.width / 2, 0};
                platformPositions[0] = {center.x - platformImage.width / 2, GetScreenHeight() - 50.f};
                platformPositions[1] = {platformPositions[0].x + platformImage.width, GetScreenHeight() - 50.f};

                pipes[0].SetRotation(0);
                pipes[1].SetRotation(180);
                pipes[2].SetRotation(0);
                pipes[3].SetRotation(180);

                float newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                pipes[0].SetPosition({backgroundPosition.x + backgroundImage.width, newY + 100});
                pipes[1].SetPosition({backgroundPosition.x + backgroundImage.width, newY});
                newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                pipes[2].SetPosition({backgroundPosition.x + backgroundImage.width + 188, newY + 100});
                pipes[3].SetPosition({backgroundPosition.x + backgroundImage.width + 188, newY});
            }

            void Input() override
            {
                if(!gameStarted && !gameOver && (IsMouseButtonPressed(0) || IsKeyPressed(KEY_SPACE)))
                {
                    gameStarted = true;
                }
                if(gameStarted && !gameOver) bird.Input();
            }

            void Update() override
            {
                if(gameStarted && !gameOver) {
                    for(int i=0; i < 4; i++) {
                        if(CheckCollisionRecs(bird.GetRectangle(), pipes[i].GetRectangle()))
                        {
                            gameOver = true;
                            gameStarted = false;
                        }

                        pipes[i].Update();
                    }

                    if(pipes[0].GetCenter().x == bird.GetPosition().x || pipes[2].GetCenter().x == bird.GetPosition().x)
                    {
                        score++;
                    }

                    for(int i=0; i < 2; i++) {
                        if(CheckCollisionRecs(bird.GetRectangle(), {platformPositions[i].x, platformPositions[i].y, (float)platformImage.width, (float)platformImage.height}))
                        {
                            gameOver = true;
                            gameStarted = false;
                            bird.Disable();
                        }
                    }

                    if(!pipes[0].InScreen(backgroundPosition.x)) {
                        float newX = backgroundPosition.x + backgroundImage.width;
                        newX += 188 - fabs(newX - pipes[2].GetPosition().x);
                        float newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                        pipes[0].SetPosition({newX, newY + 100});
                        pipes[1].SetPosition({newX, newY});
                    }

                    if(!pipes[2].InScreen(backgroundPosition.x)) {
                        float newX = backgroundPosition.x + backgroundImage.width;
                        newX += 188 - fabs(newX - pipes[0].GetPosition().x);
                        float newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                        pipes[2].SetPosition({newX, newY + 100});
                        pipes[3].SetPosition({newX, newY});
                    }


                    platformPositions[0].x -= 2;
                    platformPositions[1].x -= 2;
                    if(platformPositions[0].x + platformImage.width < center.x - platformImage.width / 2)
                    {
                        platformPositions[0].x = platformPositions[1].x + platformImage.width;
                    }
                    if(platformPositions[1].x + platformImage.width < center.x - platformImage.width / 2)
                    {
                        platformPositions[1].x = platformPositions[0].x + platformImage.width;
                    }

                    bird.Update();
                } else if(gameOver) {
                    for(int i=0; i < 2; i++) {
                        if(CheckCollisionRecs(bird.GetRectangle(), {platformPositions[i].x, platformPositions[i].y, (float)platformImage.width, (float)platformImage.height}))
                        {
                            gameOver = true;
                            gameStarted = false;
                            bird.Disable();
                        }
                    }
                    bird.Update();
                }
            }

            void Render() override
            {
                ClearBackground(BLACK);
                DrawTextureV(backgroundImage, backgroundPosition, WHITE);

                for(int i=0; i < 4; i++) {
                    pipes[i].Render();
                }

                bird.Render();
                DrawTextureV(platformImage, platformPositions[0], WHITE);
                DrawTextureV(platformImage, platformPositions[1], WHITE);

                if(gameStarted || gameOver) {
                    int textWidth = MeasureText(std::to_string(score).c_str(), 36);
                    DrawText(std::to_string(score).c_str(), center.x - textWidth / 2, center.y * 0.3f, 36, WHITE);
                }

                if(gameOver) {
                    if(GuiButton({center.x - 50, GetScreenHeight() * 0.7f, 100, 25}, "Restart"))
                    {
                        Restart();
                    }
                    if(GuiButton({center.x - 50, GetScreenHeight() * 0.8f, 100, 25}, "Close"))
                    {
                        Close();
                    }
                }

                DrawRectangle(0, 0, backgroundPosition.x, GetScreenHeight(), BLACK);
                DrawRectangle(GetScreenWidth() / 2 + backgroundImage.width / 2, 0, GetScreenWidth() / 2 - backgroundImage.width / 2, GetScreenHeight(), BLACK);
            }

        private:
            void Restart()
            {
                score = 0;
                gameStarted = false;
                gameOver = false;
                float newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                pipes[0].SetPosition({backgroundPosition.x + backgroundImage.width, newY + 96});
                pipes[1].SetPosition({backgroundPosition.x + backgroundImage.width, newY});
                newY = GetRandomValue(GetScreenHeight() * 0.2, GetScreenHeight() * 0.6);
                pipes[2].SetPosition({backgroundPosition.x + backgroundImage.width + 188, newY + 96});
                pipes[3].SetPosition({backgroundPosition.x + backgroundImage.width + 188, newY});
                bird.Reset();
            }

        private:
            Texture2D LoadBackground()
            {
                const char *currentDir = GetWorkingDirectory();
                const char *resourcePath = "resources\\Flappy Bird\\Game Objects\\background-day.png";
                const char *texturePath = TextFormat("%s/%s", currentDir, resourcePath); 
                return LoadTexture(texturePath);
            }

            Texture2D LoadPlatform()
            {
                const char *currentDir = GetWorkingDirectory();
                const char *resourcePath = "resources\\Flappy Bird\\Game Objects\\base.png";
                const char *texturePath = TextFormat("%s/%s", currentDir, resourcePath); 
                return LoadTexture(texturePath);
            }

        private:
            Vector2 center;

            int score = 0;

            bool gameStarted = false;
            bool gameOver = false;

            Texture2D backgroundImage;
            Texture2D platformImage;

            Vector2 backgroundPosition;
            Vector2 platformPositions[2];

            Pipe pipes[4];
            Bird bird;
    };
} // namespace FlappyBird


#endif
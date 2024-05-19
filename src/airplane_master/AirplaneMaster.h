#ifndef _AIRPLANE_MASTER_H_
#define _AIRPLANE_MASTER_H_

#include <vector>

#include "raylib.h"
#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

#include "../AppBase.h"

#include "TextureLoader.h"
#include "Airplane.h"
#include "Rocket.h"
#include "Shild.h"

namespace AirplaneMaster
{
    class AirplaneMaster : public AppBase {
        public:
            AirplaneMaster() : airplane(camera)
            {
                SetRandomSeed(time(NULL));

                center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

                camera.target = {0, 0};
                camera.offset = center;
                camera.rotation = 0.0f;
                camera.zoom = 1.0f;
            }

            ~AirplaneMaster()
            {
                Reset();
                TextureLoader::Release();
            }

            void Input() override 
            {
                if(gameStarted && !gameOver) {
                    airplane.Input();
                }
            }

            void Update() override
            {
                if(gameStarted && !gameOver) {
                    gameTime += GetFrameTime();

                    if(!spawned) {
                        spawnTimer += GetFrameTime();
                        if(spawnTimer >= 1.0f) {
                            spawnTimer = 0;
                            SpawnRocket();
                            spawnedCount++;
                            if(spawnedCount == currentLevel) {
                                spawned = true;
                            }
                        }
                    }

                    airplane.Update();
                    camera.target = airplane.GetPosition();
                    camera.target.x = Clamp(camera.target.x, minX, maxX);
                    camera.target.y = Clamp(camera.target.y, minY, maxY);

                    if(!airplane.IsShildActive()) {
                        for(int i=0; i < shields.size(); i++) {
                            if(CheckCollisionCircles(shields[i]->GetPosition(), shields[i]->GetRadius(), airplane.GetPosition(), airplane.GetRadius()))
                            {
                                delete shields[i];
                                shields.erase(shields.begin() + i);
                                airplane.ActivateShild();
                                break;
                            }
                        }
                    }

                    closestShield = -1;
                    closestDistance = 1000000;
                    for(int i=0; i < shields.size(); i++)
                    {
                        if(Vector2Distance(airplane.GetPosition(), shields[i]->GetPosition()) < closestDistance) {
                            closestShield = i;
                            closestDistance = Vector2Distance(airplane.GetPosition(), shields[i]->GetPosition());
                        }
                    }

                    bool allDestroyed = spawned;
                    for(int i=0; i < rockets.size(); i++)
                    {
                        if(!rockets[i]->Destroyed()) {
                            rockets[i]->Update();
                            allDestroyed = false;
                            if(!rockets[i]->Exploded() && Vector2Distance(rockets[i]->GetPosition(), airplane.GetPosition()) <= 32) {
                                if(CheckRectangleCollision(rockets[i]->GetSides(), airplane.GetSidesHorizontal()) || CheckRectangleCollision(rockets[i]->GetSides(), airplane.GetSidesVertical()))
                                {
                                    rockets[i]->Explode();
                                    if(airplane.IsShildActive()) {
                                        airplane.DeactivateShild();
                                    } else {
                                        gameOver = true;
                                        gameStarted = false;
                                    }
                                }   
                            }
                        }
                    }

                    if(allDestroyed) {
                        currentLevel++;
                        for(int i=0; i < rockets.size(); i++)
                        {
                            delete rockets[i];
                            rockets[i] = nullptr;
                        }
                        rockets.clear();
                        spawned = false;
                        spawnTimer = 0;
                        spawnedCount = 0;
                        SpawnShield();
                    }
                }
            }

            void Render() override
            {
                ClearBackground({135, 206, 235});
                if(gameOver) {
                    RenderGameOver();
                } else if(gameStarted) {
                    RenderGame();
                } else {
                    RenderMenu();
                }
            }

        private:
            void RenderMenu()
            {
                    if (GuiButton({center.x - 50, center.y, 100, 20}, "Start"))
                    {
                        Restart();   
                    }
                    else if (GuiButton({center.x - 50, center.y + 30, 100, 20}, "Close"))
                    {
                        Close();
                    }
            }

            void RenderGameOver()
            {
                const char* timeStr = std::to_string(static_cast<int>(gameTime)).c_str();
                int textWidth = MeasureText(timeStr, 36);
                DrawText(timeStr, center.x - textWidth / 2, 10, 36, WHITE);
                if (GuiButton({center.x - 50, center.y, 100, 20}, "Restart"))
                {
                    Restart();
                }
                if (GuiButton({center.x - 50, center.y + 30, 100, 20}, "Close"))
                {
                    Close();
                }
            }

            void RenderGame()
            {
                BeginMode2D(camera);
                airplane.Render();
                for(int i=0; i < rockets.size(); i++)
                {
                    rockets[i]->Render();
                }

                for(int i=0; i < shields.size(); i++)
                {
                    shields[i]->Render();
                }
                if(closestShield != -1) {
                    Vector2 airplanePosition = airplane.GetPosition();
                    Vector2 shieldPosition = shields[closestShield]->GetPosition();

                    float targetRotation = atan2f(airplanePosition.y - shieldPosition.y, airplanePosition.x - shieldPosition.x) * RAD2DEG - 90;

                    // Base width and height of the triangle (adjust for desired size)
                    float baseWidth = 8.0f;
                    float triangleHeight = 12.0f;
                    
                    float offsetDistance = airplane.GetRadius() * 1.3; // Add this line for offset distance

                    // Calculate triangle vertices
                    Vector2 directionToShield = Vector2Subtract(shieldPosition, airplanePosition);
                    directionToShield = Vector2Normalize(directionToShield);
                    Vector2 offsetPosition = { airplanePosition.x + directionToShield.x * offsetDistance, airplanePosition.y + directionToShield.y * offsetDistance };

                    // Calculate triangle vertices
                    Vector2 v1 = {offsetPosition.x + sinf(targetRotation * DEG2RAD) * triangleHeight, offsetPosition.y - cosf(targetRotation * DEG2RAD) * triangleHeight};
                    Vector2 v2 = {offsetPosition.x - cosf(targetRotation * DEG2RAD) * baseWidth / 2, offsetPosition.y - sinf(targetRotation * DEG2RAD) * baseWidth / 2};
                    Vector2 v3 = {offsetPosition.x + cosf(targetRotation * DEG2RAD) * baseWidth / 2, offsetPosition.y + sinf(targetRotation * DEG2RAD) * baseWidth / 2};

                    // Draw the triangle
                    DrawTriangle(v1, v2, v3, BLUE);
                }

                EndMode2D();

                const char* timeStr = std::to_string(static_cast<int>(gameTime)).c_str();
                int textWidth = MeasureText(timeStr, 36);
                DrawText(timeStr, center.x - textWidth / 2, 10, 36, WHITE);
            }

            void Reset()
            {
                gameStarted = false;
                gameOver = false;
                spawned = true;
                spawnTimer = 0;
                spawnedCount = 0;
                camera.target = {0, 0};
                camera.offset = center;
                airplane.Reset();
                currentLevel = 0;
                gameTime = 0;
                for(int i=0; i < rockets.size(); i++)
                {
                    delete rockets[i];
                    rockets[i] = nullptr;
                }
                rockets.clear();

                for(int i=0; i < shields.size(); i++)
                {
                    delete shields[i];
                    shields[i] = nullptr;
                }
                shields.clear();
            }

            void Restart()
            {
                Reset();
                gameStarted = true;
            }

            void SpawnRocket()
            {
                Vector2 position = {airplane.GetPosition().x + GetRandomValue(-GetScreenWidth(), GetScreenWidth()), airplane.GetPosition().y + GetRandomValue(-GetScreenHeight(), GetScreenHeight())};
                Rocket* rocket = new Rocket(position, 10.0f + (currentLevel * 0.5), airplane);
                rockets.push_back(rocket);
            }

            void SpawnShield()
            {
                Vector2 position = {GetRandomValue(minX + 100, maxX - 100), GetRandomValue(minY + 100, maxY - 100)};
                Shield* shield = new Shield(position);
                shields.push_back(shield);
            }

            bool LineLineIntersection(Vector2 A, Vector2 B, Vector2 C, Vector2 D, Vector2 *intersection = nullptr) {
                // ... (Реализация алгоритма проверки пересечения отрезков)
                // Вы можете найти готовые реализации в интернете или использовать библиотеки
                // Например, Raylib предоставляет функцию GetCollisionRecs()
                Vector2 collisionPoint;
                return CheckCollisionLines(A, B, C, D, &collisionPoint);
            }

            bool CheckRectangleCollision(std::vector<Vector2> rect1, std::vector<Vector2> rect2) {
                // Проходим по всем сторонам первого прямоугольника
                for (int i = 0; i < 4; i++) {
                    Vector2 A = rect1[i];
                    Vector2 B = rect1[(i + 1) % 4]; // Следующая вершина (циклически)

                    // Проходим по всем сторонам второго прямоугольника
                    for (int j = 0; j < 4; j++) {
                        Vector2 C = rect2[j];
                        Vector2 D = rect2[(j + 1) % 4];

                        // Проверяем пересечение отрезков AB и CD
                        if (LineLineIntersection(A, B, C, D)) {
                            return true; // Пересечение найдено, прямоугольники сталкиваются
                        }
                    }
                }

                return false; // Пересечений не найдено
            }


        private:
            Camera2D camera;

            Vector2 center;

            bool gameStarted = false;
            bool gameOver = false;
            int currentLevel = 0;
            float gameTime = 0;

            Airplane airplane;

            std::vector<Rocket*> rockets;
            bool spawned = true;
            int spawnedCount = 0;
            float spawnTimer = 0;

            std::vector<Shield*> shields;
            int closestShield = -1;
            float closestDistance = 1000000;
            float minX = -1000, minY = -1000, maxX = 1000, maxY = 1000;
    };
} // namespace AirplaneMaster


#endif
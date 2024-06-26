#ifndef _ASTEROIDS_SHIP_H_
#define _ASTEROIDS_SHIP_H_

#include <cmath>
#include "raylib.h"
#include "BulletSpawner.h"
#include "EventDispatcher.h"

namespace Asteroids
{
    class Ship {
        public:
            Ship(BulletSpawner& bulletSpawner) : bulletSpawner(bulletSpawner) {
                position = {GetScreenWidth() / 2.f, GetScreenHeight() / 2.f};
                acceleration = 0;
                rotation = 0;
                shipHeight= size / 2 / tanf(20 * DEG2RAD);
            }

            void Input()
            {
                if(IsKeyDown(KEY_A)) {
                    rotation -= 3;
                }
                if(IsKeyDown(KEY_D)) {
                    rotation += 3;
                }

                if(IsKeyDown(KEY_W)) {
                    if(acceleration < 1) acceleration += 0.04f;
                }
                else {
                    if(acceleration > 0) acceleration -= 0.02f;
                    else if(acceleration < 0) acceleration = 0;
                }
                if(IsKeyDown(KEY_S)) {
                    if(acceleration > 0) acceleration -= 0.04f;
                }

                if(IsKeyPressed(KEY_SPACE)) {
                    Vector2 bulletSpawnPosition = {position.x + sinf(rotation * DEG2RAD) * shipHeight, position.y - cosf(rotation * DEG2RAD) * shipHeight};
                    bulletSpawner.Spawn(bulletSpawnPosition, rotation);
                }
            }

            void Update(AsteroidSpawner& asteroidSpawner)
            {
                position.x += acceleration * sin(rotation * DEG2RAD) * speed;
                position.y -= acceleration * cos(rotation * DEG2RAD) * speed;

                if(position.x < -shipHeight / 2) {
                    position.x = GetScreenWidth() + shipHeight / 2;
                }
                if(position.x > GetScreenWidth() + shipHeight / 2) {
                    position.x = -shipHeight / 2;
                }
                if(position.y < -shipHeight / 2) {
                    position.y = GetScreenHeight() + shipHeight / 2;
                }
                if(position.y > GetScreenHeight() + shipHeight / 2) {
                    position.y = -shipHeight / 2;
                }

                if(!isInvulnerable) {
                    Vector3 collider = (Vector3){position.x + sin(rotation*DEG2RAD)*(shipHeight/2.5f), position.y - cos(rotation*DEG2RAD)*(shipHeight/2.5f), 12};
                    for(auto& asteroid : asteroidSpawner.GetAsteroids())
                    {
                        if(asteroid.IsActive() && CheckCollisionCircles({collider.x, collider.y}, collider.z, asteroid.GetPosition(), asteroid.GetRadius()))
                        {
                            ShipDestroyedEvent event;
                            EventDispatcher::DispatchEvent(event);
                            break;
                        }
                    }
                } else {
                    invulnerableTimer += GetFrameTime();
                    if(invulnerableTimer >= invulnerableDuration) {
                        isInvulnerable = false;
                    }
                }
            }

            void Render()
            {
                Vector2 v1 = {position.x + sinf(rotation * DEG2RAD) * shipHeight, position.y - cosf(rotation * DEG2RAD) * shipHeight};
                Vector2 v2 = {position.x - cosf(rotation * DEG2RAD) * size / 2, position.y - sinf(rotation * DEG2RAD) * size / 2};
                Vector2 v3 = {position.x + cosf(rotation * DEG2RAD) * size / 2, position.y + sinf(rotation * DEG2RAD) * size / 2};

                if(isInvulnerable) {
                    blinkTimer += GetFrameTime();
                    if(blinkTimer >= blinkInterval) {
                        DrawTriangle(v1, v2, v3, {255, 255, 255, 0});
                        blinkTimer = 0;
                    } else {
                        DrawTriangle(v1, v2, v3, WHITE);
                    }
                } else {
                    DrawTriangle(v1, v2, v3, WHITE);
                }
            }

            void Reset()
            {
                position = {GetScreenWidth() / 2.f, GetScreenHeight() / 2.f};
                acceleration = 0;
                rotation = 0;
            }

            void Invulnerable()
            {
                isInvulnerable = true;
                invulnerableTimer = 0.0f;
            }
        
        private:
            Vector2 position;
            float acceleration = 0;
            float rotation = 0;
            float size = 20;
            float speed = 3;
            float shipHeight = 0;

            bool isInvulnerable = false;
            float invulnerableDuration = 2.0f;
            float invulnerableTimer = 0.0f;

            float blinkTimer = 0.0f;
            float blinkInterval = 0.25f;

            BulletSpawner& bulletSpawner;

    };
} // namespace Asteroids



#endif
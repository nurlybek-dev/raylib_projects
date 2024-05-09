#ifndef _ASTEROIDS_ASTEROID_H_
#define _ASTEROIDS_ASTEROID_H_

#include "cmath"
#include "raylib.h"


namespace Asteroids
{
    class Asteroid {
        public:
            Asteroid()
            {
                position = {0, 0};
                size = 3;
                radius = size * 12;
                rotation = 0;
                angularVelocity = 0;
                linearVelocity = 0;
                isActive = false;
                destroyedLastFrame = false;
            }

            void Initialize(int size)
            {
                this->size = size;
                radius = this->size * 6;
                rotation = GetRandomValue(-360, 360);
                angularVelocity = GetRandomValue(-3, 3);
                linearVelocity = GetRandomValue(-3, 3);
                
                do {
                    linearVelocity = rand() % 3 + 1;
                    linearVelocity *= (rand() % 2) ? 1 : -1;
                } while(linearVelocity == 0);

                position = {GetRandomValue(0, GetScreenWidth()) * 1.f, GetRandomValue(0, GetScreenHeight()) * 1.f};
                isActive = true;
                destroyedLastFrame = false;
            }

            void Initialize(Vector2 position, float linearVelocity, int size)
            {
                this->size = size;
                printf("New size: %d\n", this->size);
                radius = this->size * 6;
                rotation = GetRandomValue(-360, 360);
                angularVelocity = GetRandomValue(-3, 3);
                this->linearVelocity = linearVelocity * ((float)rand() / RAND_MAX + 0.5f);
                this->position = position;
                isActive = true;
                destroyedLastFrame = false;
            }

            void Update()
            {
                if(isActive)
                {
                    position.x += linearVelocity * sin(rotation * DEG2RAD);
                    position.y += linearVelocity * cos(rotation * DEG2RAD);
                    if(position.x < 0) {
                        position.x = GetScreenWidth();
                    }
                    if(position.x > GetScreenWidth()) {
                        position.x = 0;
                    }
                    if(position.y < 0) {
                        position.y = GetScreenHeight();
                    }
                    if(position.y > GetScreenHeight()) {
                        position.y = 0;
                    }
                }
            }

            void Render()
            {
                if(isActive) {
                    DrawCircle(position.x, position.y, radius, WHITE);
                }
            }

            Vector2 GetPosition()
            {
                return position;
            }

            float GetRadius()
            {
                return radius;
            }

            float GetLinerVelocity()
            {
                return linearVelocity;
            }

            bool IsActive()
            {
                return isActive;
            }

            void Destroy()
            {
                destroyedLastFrame = true;
            }

            bool Destroyed()
            {
                return destroyedLastFrame;
            }

            void Disable()
            {
                isActive = false;
                destroyedLastFrame = false;
            }
        
            int GetSize()
            {
                return size;
            }

        private:
            Vector2 position;
            float linearVelocity;
            float radius;
            float rotation;
            float angularVelocity;
            bool isActive = false;
            bool destroyedLastFrame = false;
            int size = 3;
        
    };
} // namespace Asteroids


#endif
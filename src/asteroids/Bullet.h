#ifndef _ASTEROIDS_BULLET_H_
#define _ASTEROIDS_BULLET_H_

#include <vector>
#include <raylib.h>

namespace Asteroids
{
    class Bullet {
    public:
        Bullet()
        {
            isActive = false;
            position = {0, 0};
            velocity = {0, 0};
        }

        void Initialize(Vector2 position, float rotation)
        {
            this->position = position;
            velocity.x = sin(rotation * DEG2RAD) * speed;
            velocity.y = cos(rotation * DEG2RAD) * speed;
            isActive = true;
        }

        void Update()
        {
            if(isActive) {
                position.x += velocity.x;
                position.y -= velocity.y;

                if(position.x < -radius || position.x > GetScreenWidth() + radius || position.y < -radius || position.y > GetScreenHeight() + radius) {
                    isActive = false;
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

        bool IsActive()
        {
            return isActive;
        }

        void Destroy()
        {
            isActive = false;
        }
    
    private:
        Vector2 position;
        Vector2 velocity;
        float radius = 2;
        bool isActive;
        float speed = 6;
};
} // namespace Asteroids



#endif
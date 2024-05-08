#ifndef _PING_PONG_BALL_H_
#define _PING_PONG_BALL_H_

#include "raylib.h"
#include <cmath>


namespace PingPong
{
    class Ball {
        public:
            Ball() 
            {
                bounds = {roundf(GetScreenWidth() / 2), roundf(GetScreenHeight() / 2), 16, 16};
                velocity = {0.75, 0.25};
                ballSpeed = 6;
            }

            void Update(Rectangle paddle1, Rectangle paddle2)
            {
                bounds.x += velocity.x * ballSpeed;
                bounds.y += velocity.y * ballSpeed;

                if(bounds.x < 0) {
                    bounds.x = 0;
                    velocity.x *= -1;
                } else if (bounds.x > GetScreenWidth()) {
                    bounds.x = GetScreenWidth();
                    velocity.x *= -1;
                }

                if(bounds.y < 0) {
                    bounds.y = 0;
                    velocity.y *= -1;
                } else if (bounds.y + bounds.height > GetScreenHeight()) {
                    bounds.y = GetScreenHeight() - bounds.height;
                    velocity.y *= -1;
                }
                if(CheckCollisionRecs(bounds, paddle1) || CheckCollisionRecs(bounds, paddle2)) {
                    velocity.x *= -1;
                    ballSpeed = 12;
                }
            }

            void Render()
            {
                DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, WHITE);
            }

            Vector2 Position()
            {
                return {bounds.x, bounds.y};
            }

            void Reset()
            {
                bounds.x = roundf(GetScreenWidth() / 2);
                bounds.y = roundf(GetScreenHeight() / 2);
                velocity = {0.75, 0.25};
                ballSpeed = 6;
            }

            void SetVelocity(Vector2 newVelocity)
            {
                velocity = newVelocity;
            }

        private:
            int ballSpeed;
            Rectangle bounds;
            Vector2 velocity;
    };
} // namespace PingPong


#endif
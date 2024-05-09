#ifndef _ARKANOID_BALL_H_
#define _ARKANOID_BALL_H_

#include <vector>
#include <cmath>
#include "raylib.h"

#include "Brick.h"


namespace Arkanoid
{
    class Ball {
        public:
            Ball() 
            {
                radius = 8;
                position = {roundf(GetScreenWidth() / 2), roundf(GetScreenHeight() / 2)};
                velocity = {0, 0};
                ballSpeed = 6;
            }

            void Update(Rectangle paddle, std::vector<Brick*>& bricks)
            {
                position.x += velocity.x * ballSpeed;
                position.y += velocity.y * ballSpeed;

                if(position.x < 0) {
                    position.x = 0;
                    velocity.x *= -1;
                } else if (position.x > GetScreenWidth()) {
                    position.x = GetScreenWidth();
                    velocity.x *= -1;
                }

                if(position.y < 0) {
                    position.y = 0;
                    velocity.y *= -1;
                }

                if(CheckCollisionCircleRec(position, radius, paddle)) {
                    float d = (position.x - (paddle.x + paddle.width / 2.0f)) / paddle.width * 1.6;
                    velocity.y *= -1;
                    velocity.x = d;
                }

                for(auto& brick : bricks)
                {
                    if(CheckCollisionCircleRec(position, radius, brick->GetBounds())) {
                        if(brick->IsActive())
                        {
                            Rectangle brickBounds = brick->GetBounds();
                            float d = (position.x - (brickBounds.x + brickBounds.width / 2.0f)) / brickBounds.width;
                            velocity.y *= -1;
                            velocity.x = d;
                            brick->Disable();
                            break;
                        }
                    }
                }
            }

            void Render()
            {
                DrawCircle(position.x, position.y, radius, WHITE);
            }

            Vector2 Position()
            {
                return {position.x, position.y};
            }

            void Reset()
            {
                position.x = roundf(GetScreenWidth() / 2);
                position.y = roundf(GetScreenHeight() / 2);
                velocity = {0, 0};
            }

            void SetPosition(Vector2 newPosition)
            {
                position = newPosition;
            }

            void SetVelocity(Vector2 newVelocity)
            {
                velocity = newVelocity;
            }

        private:
            int ballSpeed;
            Vector2 position;
            float radius;
            Vector2 velocity;
    };
} // namespace Arkanoid


#endif
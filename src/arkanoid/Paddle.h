#ifndef _ARKANOID_PADDLE_H_
#define _ARKANOID_PADDLE_H_

#include <raylib.h>
#include <cmath>

namespace Arkanoid
{
    class Paddle {
        public:
            Paddle() {
                bounds = {roundf(GetScreenWidth() / 2 - 50), roundf(GetScreenHeight() * 7/8), 100, 10};
                paddleSpeed = 5;
            }
            void Input()
            {
                velocity.x = 0;
                if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                    velocity.x -= 1;
                } else if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                    velocity.x += 1;
                }
            }

            void Update()
            {
                bounds.x += velocity.x * paddleSpeed;

                if(bounds.x < 0) {
                    bounds.x = 0;
                } else if(bounds.x + bounds.width > GetScreenWidth()) {
                    bounds.x = GetScreenWidth() - bounds.width;
                }
            }

            void Render()
            {
                DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, WHITE);
            }

            Rectangle GetBounds()
            {
                return bounds;
            }

            Vector2 GetPosition()
            {
                return {bounds.x, bounds.y};
            }

            void Reset()
            {
                bounds.x = GetScreenWidth() / 2 - 50;
            }

        private:
            float paddleSpeed;
            Rectangle bounds;
            Vector2 velocity;

    };
} // namespace Arkanoid


#endif
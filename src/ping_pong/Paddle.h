#ifndef _PING_PONG_PADDLE_H_
#define _PING_PONG_PADDLE_H_

namespace PingPong
{
    class Paddle {
        public:
            Paddle() {
                bounds = {0, 0, 10, 100};
                paddleSpeed = 3;
            }
            void Input(Vector2 ballPosition)
            {
                velocity.y = 0;
                if(id == 1) {
                    if(IsKeyDown(KEY_W)) {
                        velocity.y -= 1;
                    } else if(IsKeyDown(KEY_S)) {
                        velocity.y += 1;
                    }
                } else if(id == 2) {
                    if(IsKeyDown(KEY_UP)) {
                        velocity.y -= 1;
                    } else if(IsKeyDown(KEY_DOWN)) {
                        velocity.y += 1;
                    }
                } else {
                    int yCenter = bounds.y + bounds.height / 2;
                    if(yCenter - ballPosition.y < -5) {
                        velocity.y = 1;
                    } else if(yCenter - ballPosition.y > 5) {
                        velocity.y = -1;
                    }
                }
            }

            void Update()
            {
                bounds.y += velocity.y * paddleSpeed;

                if(bounds.y < 0) {
                    bounds.y = 0;
                } else if(bounds.y + bounds.height > GetScreenHeight()) {
                    bounds.y = GetScreenHeight() - bounds.height;
                }
            }

            void Render()
            {
                DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, WHITE);
            }

            void SetPlayer(int number)
            {
                id = number;
                if(id == 1) {
                    bounds.x = 20;
                    bounds.y = GetScreenHeight() / 2 - bounds.height;
                } else if(id == 2) {
                    bounds.x = GetScreenWidth() - bounds.width - 20;
                    bounds.y = GetScreenHeight() / 2 - bounds.height;
                }
            }

            void SetAI()
            {
                isAI = true;
                bounds.x = GetScreenWidth() - bounds.width - 20;
                bounds.y = GetScreenHeight() / 2 - bounds.height;
            }

            Rectangle GetBounds()
            {
                return bounds;
            }

            void Reset()
            {
                if(id == 1) {
                    bounds.x = 20;
                    bounds.y = GetScreenHeight() / 2 - bounds.height;
                } else if(id == 2) {
                    bounds.x = GetScreenWidth() - bounds.width - 20;
                    bounds.y = GetScreenHeight() / 2 - bounds.height;
                }  
            }

        private:
            int id;
            bool isAI;
            float paddleSpeed;
            Rectangle bounds;

            Vector2 position;
            Vector2 velocity;

    };
} // namespace PingPong


#endif
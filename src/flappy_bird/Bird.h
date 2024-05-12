#ifndef _FLAPPY_BIRD_BIRD_H_
#define _FLAPPY_BIRD_BIRD_H_

#include "raylib.h"
#include "raymath.h"

namespace FlappyBird
{
    class Bird
    {
        public:
            Bird()
            {
                texture = LoadBird();
                position = {GetScreenWidth() * 0.4f, GetScreenHeight() / 2.0f - texture.width / 2};
            }

            void Input()
            {
                if(disabled) return;
                if(IsMouseButtonPressed(0) || IsKeyPressed(KEY_SPACE))
                {
                    jump = true;
                }
            }

            void Update()
            {
                if(disabled) return;
                velocity += gravityForce * GetFrameTime();
                velocity = Clamp(velocity, -jumpForce, gravityForce);
                if(jump) {
                    velocity = -jumpForce;
                    jump = false;
                }

                if(velocity < 0) {
                    rotation = -30;
                } else if (velocity == gravityForce) {
                    rotation = 90;
                } else {
                    rotation += 5;
                }
                rotation = Clamp(rotation, -30, 90);

                position.y += velocity;
            }

            void Render()
            {
                DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, {position.x, position.y, (float)texture.width, (float)texture.height}, {texture.width / 2.0f, texture.height / 2.0f}, rotation, WHITE);
            }
        
            Rectangle GetRectangle()
            {
                return {position.x - texture.width / 2, position.y - texture.height / 2, texture.width, texture.height};
            }

            void Disable()
            {
                disabled = true;
            }

            void Reset()
            {
                disabled = false;
                position = {GetScreenWidth() * 0.4f, GetScreenHeight() / 2.0f - texture.width / 2};
                jump = false;
                rotation = 0;
                velocity = 0;
            }

            Vector2 GetPosition()
            {
                return position;
            }

        private:
            Texture2D LoadBird()
            {
                const char *currentDir = GetWorkingDirectory();
                const char *resourcePath = "resources\\Flappy Bird\\Game Objects\\yellowbird-midflap.png";
                const char *texturePath = TextFormat("%s/%s", currentDir, resourcePath); 
                return LoadTexture(texturePath);
            }
        private:
            Texture2D texture;
            
            bool disabled = false;

            Vector2 position;
            float rotation = 0;
            float velocity = 0;

            bool jump = false;
            float jumpForce = 4;
            float gravityForce = 9.8;
    };
} // namespace FlappyBird
 

#endif
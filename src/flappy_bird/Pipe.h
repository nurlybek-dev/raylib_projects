#ifndef _FLAPPY_BIRD_PIPE_H_
#define _FLAPPY_BIRD_PIPE_H_

#include "raylib.h"

namespace FlappyBird
{
    class Pipe
    {
        public:
            Pipe()
            {
                texture = LoadPipe();
            }

            void Update()
            {
                position.x -= 1;
            }

            void Render() 
            {
                DrawTextureEx(texture, position, rotation, 1, WHITE);
            }

            void SetPosition(Vector2 newPosition)
            {
                position = newPosition;
                if(rotation > 0) {
                    position.x += texture.width;
                }
            }

            Vector2 GetPosition()
            {
                return position;
            }

            Vector2 GetCenter()
            {
                return {position.x + texture.width / 2.0f, position.y + texture.height / 2.0f};
            }

            void SetRotation(float newRotation)
            {
                rotation = newRotation;
            }

            bool InScreen(float leftX)
            {
                if(rotation > 0) {
                    return position.x > leftX;
                } else {
                    return position.x + texture.width > leftX;
                }
            }

            Rectangle GetRectangle()
            {
                if(rotation > 0) {
                    return {position.x - texture.width, position.y - texture.height, texture.width, texture.height};
                } else {
                    return {position.x, position.y, texture.width, texture.height};
                }
            }

        private:
            Texture2D LoadPipe()
            {
                const char *currentDir = GetWorkingDirectory();
                const char *resourcePath = "resources\\Flappy Bird\\Game Objects\\pipe-green.png";
                const char *texturePath = TextFormat("%s/%s", currentDir, resourcePath); 
                return LoadTexture(texturePath);
            }
        private:
            Vector2 position;
            float rotation;
            Texture2D texture;
    };
} // namespace FlappyBird


#endif
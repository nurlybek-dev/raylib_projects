#ifndef _AIRPLANE_MASTER_SHIELD_H_
#define _AIRPLANE_MASTER_SHIELD_H_

#include <cmath>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "TextureLoader.h"

namespace AirplaneMaster
{
    class Shield {
        public:
            Shield(Vector2 position) : position(position)
            {
                texture = TextureLoader::GetTexture("shield.png");
            }

            void Render()
            {
                Vector2 pos = GetPosition();
                Rectangle destRec = {pos.x, pos.y, (float)texture.width * scale, (float)texture.height * scale};
                Rectangle sourceRec = {0, 0, texture.width, texture.height};
                Vector2 origin = {texture.width * scale / 2, texture.height * scale / 2}; 
                DrawTexturePro(texture, sourceRec, destRec, origin, rotation, WHITE);
            }

            Vector2 GetPosition()
            {
                return {position.x + texture.width * scale / 2, position.y + texture.height * scale / 2};
            }

            float GetRadius()
            {
                return texture.width * scale / 2;
            }

        private:
            Texture2D texture;
            Vector2 position = {0, 0};
            float scale = 1.5;
            float rotation = 0;
    };
} // namespace AirplaneMaster


#endif
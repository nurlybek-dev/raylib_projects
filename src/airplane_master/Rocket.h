#ifndef _AIRPLANE_MASTER_ROCKET_H_
#define _AIRPLANE_MASTER_ROCKET_H_

#include <cmath>
#include "raylib.h"
#include "raymath.h"

#include "TextureLoader.h"
#include "Airplane.h"

namespace AirplaneMaster
{
    class Rocket {
        public:
            Rocket(Vector2 position, float destructionTimer, Airplane& airplane) : position(position), destructionTimer(destructionTimer), airplane(airplane)
            {
                texture = TextureLoader::GetTexture("rocket.png");

                explodeFrame = 0;
                explodeTextures[0] = TextureLoader::GetTexture("explode_1.png");
                explodeTextures[1] = TextureLoader::GetTexture("explode_2.png");
                explodeTextures[2] = TextureLoader::GetTexture("explode_3.png");
                
                lerpSpeed = lerpSpeed + GetRandomValue(-100, 100) / 1000.0f;

                Vector2 airplanePosition = airplane.GetPosition();
                Vector2 texturePos = GetPosition();
                rotation = atan2f(airplanePosition.y - texturePos.y, airplanePosition.x - texturePos.x) * RAD2DEG;
            }

            ~Rocket()
            {
                
            }

            void Update()
            {
                if(destroyed) return;
                if(exploded) {
                    if(explodeFrame >= 30) {
                        Destroy();
                    }
                    explodeFrame++;
                    return;
                }
                destructionTimer -= GetFrameTime();
                if(destructionTimer <= 0) Explode();
                Vector2 airplanePosition = airplane.GetPosition();
                Vector2 texturePos = GetPosition();
                float targetRotation = atan2f(airplanePosition.y - texturePos.y, airplanePosition.x - texturePos.x) * RAD2DEG;

                // Calculate the direction vector from the current rotation
                Vector2 currentDirection = { cosf(DEG2RAD * rotation), sinf(DEG2RAD * rotation) };

                // Calculate the direction vector towards the target rotation
                Vector2 targetDirection = { cosf(DEG2RAD * targetRotation), sinf(DEG2RAD * targetRotation) };

                // Find the angle between the current and target directions
                float angleDiff = Vector2Angle(currentDirection, targetDirection);

                // Ensure the shortest rotation path
                if (angleDiff > 180.0f) {
                    angleDiff -= 360.0f;
                } else if (angleDiff < -180.0f) {
                    angleDiff += 360.0f;
                }

                // Rotate the current direction towards the target direction
                currentDirection = Vector2Rotate(currentDirection, DEG2RAD * angleDiff * lerpSpeed);

                // Update the rotation based on the new direction
                rotation = RAD2DEG * atan2f(currentDirection.y, currentDirection.x);

                position.x += cos(rotation * DEG2RAD) * speed;
                position.y += sin(rotation * DEG2RAD) * speed;
            }

            void Render()
            {
                if(destroyed) return;
                Vector2 pos = GetPosition();
                Rectangle destRec = {pos.x, pos.y, (float)texture.width * scale, (float)texture.height * scale};
                Rectangle sourceRec = {0, 0, texture.width, texture.height};
                Vector2 origin = {texture.width/2, texture.height/2}; 
                if(exploded) {
                    DrawTexturePro(explodeTextures[(explodeFrame / explodeFrameTime) % 3], sourceRec, destRec, origin, rotation, WHITE);
                } else {
                    DrawTexturePro(texture, sourceRec, destRec, origin, rotation, WHITE);
                }
            }

            void Explode()
            {
                exploded = true;
            }

            bool Exploded()
            {
                return exploded;
            }

            void Destroy()
            {
                destroyed = true;
            }

            bool Destroyed()
            {
                return destroyed;
            }

            Vector2 GetPosition()
            {
                return {position.x + texture.width / 2, position.y + texture.height / 2};
            }

            Rectangle GetBounds()
            {
                return {position.x + texture.width / 2, position.y + texture.height / 2, texture.width, texture.height};
            }

            std::vector<Vector2> GetSides()
            {
                Vector2 origin = {(texture.width - 6) / 2.f, (texture.height - 16) / 2.f};
                Rectangle rec1 = {position.x + texture.width / 2, position.y + texture.height / 2.f, (float)texture.width, (float)texture.height - 8};

                Vector2 topLeft = { 0 };
                Vector2 topRight = { 0 };
                Vector2 bottomLeft = { 0 };
                Vector2 bottomRight = { 0 };

                float sinRotation = sinf(rotation*DEG2RAD);
                float cosRotation = cosf(rotation*DEG2RAD);
                float x = rec1.x;
                float y = rec1.y;
                float dx = -origin.x;
                float dy = -origin.y;

                topLeft.x = x + dx*cosRotation - dy*sinRotation;
                topLeft.y = y + dx*sinRotation + dy*cosRotation;

                topRight.x = x + (dx + rec1.width)*cosRotation - dy*sinRotation;
                topRight.y = y + (dx + rec1.width)*sinRotation + dy*cosRotation;

                bottomLeft.x = x + dx*cosRotation - (dy + rec1.height)*sinRotation;
                bottomLeft.y = y + dx*sinRotation + (dy + rec1.height)*cosRotation;

                bottomRight.x = x + (dx + rec1.width)*cosRotation - (dy + rec1.height)*sinRotation;
                bottomRight.y = y + (dx + rec1.width)*sinRotation + (dy + rec1.height)*cosRotation;

                return {topLeft, topRight, bottomLeft, bottomRight};
            }

        private:
            Airplane& airplane;

            Texture2D texture;
            Vector2 position = {0, 0};
            float rotation = 0;
            float scale = 1.5;
            float speed = 5;
            float lerpSpeed = 1.0f;

            bool destroyed = false;
            float destructionTimer = 0.0;

            bool exploded = false;
            Texture2D explodeTextures[3];
            int explodeFrame = 0;
            int explodeFrameTime = 10;
    };
} // namespace AirplaneMaster


#endif
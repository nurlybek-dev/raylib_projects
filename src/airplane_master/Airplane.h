#ifndef _AIRPLANE_MASTER_AIRPLANE_H_
#define _AIRPLANE_MASTER_AIRPLANE_H_

#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include <iostream>

#include "TextureLoader.h"


namespace AirplaneMaster
{
    class Airplane {
        public:
            Airplane(Camera2D& camera) : camera(camera)
            {
                texture = TextureLoader::GetTexture("ship.png");
                position = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
                GetSidesVertical();
            }

            void Input()
            {
                Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
                Vector2 texturePos = GetPosition();
                targetRotation = atan2f(mousePos.y - texturePos.y, mousePos.x - texturePos.x) * RAD2DEG;
            }

            void Update()
            {
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

                // rotation = Lerp(rotation, rotation + angleDiff, lerpSpeed);
                position.x += cos(rotation * DEG2RAD) * speed;
                position.y += sin(rotation * DEG2RAD) * speed;
            }

            void Render()
            {
                Vector2 pos = GetPosition();
                Rectangle destRec = {pos.x, pos.y, (float)texture.width * scale, (float)texture.height * scale};
                Rectangle sourceRec = {0, 0, texture.width, texture.height};
                Vector2 origin = {texture.width/2, texture.height/2};

                DrawTexturePro(texture, sourceRec, destRec, origin, rotation, WHITE);
                if(shildActive) {
                    DrawCircleGradient(pos.x, pos.y, shildRadius, { 0, 82, 172, 0 }, { 0, 82, 172, 155 });
                }
            }

            void Reset()
            {
                position = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
                rotation = 0;
                targetRotation = 0;
            }

            float GetRadius()
            {
                return texture.width / 2;
            }

            Vector2 GetPosition()
            {
                return {position.x + texture.width / 2, position.y + texture.height / 2};
            }

            Rectangle GetBounds()
            {
                return {position.x, position.y, texture.width, texture.height};
            }

            std::vector<Vector2> GetSidesHorizontal()
            {
                Vector2 origin = {5, texture.height / 2.f};
                Rectangle rec1 = {position.x + texture.width / 2, position.y + texture.height / 2.f, 10, (float)texture.height};

                Vector2 topLeft = { 0 };
                Vector2 topRight = { 0 };
                Vector2 bottomLeft = { 0 };
                Vector2 bottomRight = { 0 };

                float sinRotation = sinf(rotation*DEG2RAD); // 0
                float cosRotation = cosf(rotation*DEG2RAD); // 1
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

            std::vector<Vector2> GetSidesVertical()
            {
                Vector2 origin = {(texture.width - 8) / 2.f, 3};
                Rectangle rec1 = {position.x + texture.width / 2, position.y + texture.height / 2, texture.width - 8, 6};

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

            void ActivateShild()
            {
                shildActive = true;
            }

            void DeactivateShild()
            {
                shildActive = false;
            }

            bool IsShildActive()
            {
                return shildActive;
            }

        private:
            Camera2D& camera;

            Texture2D texture;
            Vector2 position = {0, 0};
            float rotation = 0;
            float targetRotation = 0;
            float scale = 1;
            float speed = 3;
            float lerpSpeed = 1.75;

            float shildRadius = 20.0f;
            bool shildActive = true;
    };
} // namespace AirplaneMaster


#endif
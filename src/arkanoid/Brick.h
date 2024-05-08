#ifndef _ARKANOID_BRICK_H_
#define _ARKANOID_BRICK_H_

#include <raylib.h>

const int BRICK_WIDTH = 50;
const int BRICK_HEIGHT = 20;

namespace Arkanoid
{
    class Brick {
        public:
            Brick(Vector2 position, int row, int col) {
                bounds = {position.x, position.y, BRICK_WIDTH, BRICK_HEIGHT};
                color = ((row + col) % 2 == 0) ? BLUE : YELLOW;
            }

            void Render()
            {
                if(isActive) DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, color);
            }

            Rectangle GetBounds()
            {
                return bounds;
            }

            void Disable()
            {
                isActive = false;
            }

            void Reset()
            {
                isActive = true;
            }

            bool IsActive()
            {
                return isActive;
            }

        private:
            Rectangle bounds;
            bool isActive = true;
            Color color;
    };
} // namespace Arkanoid


#endif
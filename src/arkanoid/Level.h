#ifndef _ARKANOID_LEVEL_H_
#define _ARKANOID_LEVEL_H_

#include <raylib.h>
#include <vector>
#include "Brick.h"

namespace Arkanoid
{
    class Level {
        public:
            Level(int rows, int cols) {

                float startX = GetScreenWidth() / 2 - (cols / 2.0 * BRICK_WIDTH);
                float startY = GetScreenHeight() * 0.05;

                for(int y = 0; y < cols; y++) {
                    for(int x = 0; x < rows; x++) {
                        bricks.push_back(new Brick(Vector2{startX + (x * BRICK_WIDTH), startY + (y * BRICK_HEIGHT)}, x, y));
                    }
                }
            }

            ~Level()
            {
                for(int i=0; i < bricks.size(); i++)
                {
                    delete bricks[i];
                    bricks[i] = nullptr;
                }
                bricks.clear();
            }

            void Update()
            {
                isComplete = true;
                for(auto& brick: bricks)
                {
                    if(brick->IsActive())
                    {
                        isComplete = false;
                        break;
                    }
                }
            }

            void Render()
            {
                for(auto& brick: bricks)
                {
                    brick->Render();
                }
            }

            void Reset()
            {
                isActive = true;
            }

            std::vector<Brick*>& GetBricks()
            {
                return bricks;
            }

            bool IsComplete()
            {
                return isComplete;
            }

        private:
            bool isActive = true;
            bool isComplete = false;

            std::vector<Brick*> bricks;
    };
} // namespace Arkanoid


#endif
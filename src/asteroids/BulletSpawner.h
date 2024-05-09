#ifndef _ASTEROIDS_BULLET_SPAWNER_H_
#define _ASTEROIDS_BULLET_SPAWNER_H_

#include <vector>
#include <raylib.h>

#include "Bullet.h"
#include "AsteroidSpawner.h"

namespace Asteroids
{
    class BulletSpawner {
    public:
        void Update(AsteroidSpawner& asteroidSpawner)
        {
            for(auto& bullet : bullets)
            {
                bullet.Update();
                if(!bullet.IsActive()) continue;
                for(auto& asteroid : asteroidSpawner.GetAsteroids())
                {
                    if(asteroid.IsActive() && CheckCollisionCircles(bullet.GetPosition(), bullet.GetRadius(), asteroid.GetPosition(), asteroid.GetRadius()))
                    {
                        bullet.Destroy();
                        asteroid.Destroy();
                        break;
                    }
                }

            }
        }

        void Render()
        {
            for(auto& bullet : bullets)
            {
                bullet.Render();
            }
        }

        void Spawn(Vector2 position, float rotation)
        {
            bool spawned = false;
            for(auto& bullet : bullets)
            {
                if(!bullet.IsActive()) {
                    bullet.Initialize(position, rotation);
                    spawned = true;
                    break;
                }
            }
            if(!spawned) {
                Bullet newBullet;
                newBullet.Initialize(position, rotation);
                bullets.push_back(newBullet);
            }
        }

    private:
        std::vector<Bullet> bullets;
};
} // namespace Asteroids



#endif
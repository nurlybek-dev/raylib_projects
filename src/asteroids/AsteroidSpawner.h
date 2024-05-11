#ifndef _ASTEROIDS_ASTEROID_SPAWNER_H_
#define _ASTEROIDS_ASTEROID_SPAWNER_H_

#include <vector>
#include <raylib.h>

#include "Asteroid.h"
#include "EventDispatcher.h"

namespace Asteroids
{
    class AsteroidSpawner {
        private:
            static constexpr int MAX_BIG_ASTEROIDS = 4;
            static constexpr int MAX_MEDIUM_ASTEROIDS = 8;
            static constexpr int MAX_SMALL_ASTEROIDS = 16;
        
        public:
            AsteroidSpawner()
            {
                asteroids.resize(MAX_BIG_ASTEROIDS + MAX_MEDIUM_ASTEROIDS + MAX_SMALL_ASTEROIDS);
                EventDispatcher::AddEventListener(EventType::ASTEROID_DESTROYED, std::bind(&AsteroidSpawner::OnAsteroidDestroyed, this, std::placeholders::_1));
            }

            void Update()
            {
                completed = true;
                for(auto& asteroid : asteroids)
                {
                    asteroid.Update();
                    if(asteroid.IsActive()) completed = false;
                }
            }

            void Render()
            {
                for(auto& asteroid : asteroids)
                {
                    asteroid.Render();
                }
            }

            void SpawnLevel()
            {
                for(int i = 0; i < MAX_BIG_ASTEROIDS; i++)
                {
                    Spawn(3);
                }
            }

            void Spawn(int size)
            {
                for(auto& asteroid : asteroids)
                {
                    if(!asteroid.IsActive()) {
                        asteroid.Initialize(size);
                        break;
                    }
                }
            }

            void Spawn(Vector2 position, float linearVelocity, int size)
            {
                for(auto& asteroid : asteroids)
                {
                    if(!asteroid.IsActive()) {
                        asteroid.Initialize(position, linearVelocity, size);
                        break;
                    }
                }
            }

            std::vector<Asteroid>& GetAsteroids()
            {
                return asteroids;
            }

            bool Completed()
            {
                return completed;
            }

            void Reset()
            {
                for(auto& asteroid : asteroids)
                {
                    asteroid.Reset();
                }
            }

            void OnAsteroidDestroyed(Event& event)
            {
                AsteroidDestroyedEvent& asteroidEvent = dynamic_cast<AsteroidDestroyedEvent&>(event);
                Asteroid& asteroid = asteroidEvent.GetAsteroid();
                int size = asteroid.GetSize();
                if(size > 1) {
                    Spawn(asteroid.GetPosition(), asteroid.GetLinerVelocity(), size - 1);
                    Spawn(asteroid.GetPosition(), asteroid.GetLinerVelocity(), size - 1);
                }
            }

        private:
            std::vector<Asteroid> asteroids;

            bool completed;
    };
} // namespace Asteroids


#endif
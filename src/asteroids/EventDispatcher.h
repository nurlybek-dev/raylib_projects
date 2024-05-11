#ifndef _ASTEROIDS_EVENT_DISPATCHER_H_
#define _ASTEROIDS_EVENT_DISPATCHER_H_

#include <map>
#include <vector>
#include <functional>

namespace Asteroids
{
    enum EventType {
        ASTEROID_DESTROYED,
        SHIP_DESTROYED
    };

    class Event {
        public:
            virtual EventType GetType() const = 0;
    };

    class Asteroid;
    class AsteroidDestroyedEvent : public Event {
        public:
            AsteroidDestroyedEvent(Asteroid& asteroid) : asteroid(asteroid) 
            {

            }

            EventType GetType() const override 
            { 
                return EventType::ASTEROID_DESTROYED; 
            }

        Asteroid& GetAsteroid() const { return asteroid; }

        private:
            Asteroid& asteroid;
    };

    class ShipDestroyedEvent : public Event {
        public:
            EventType GetType() const override 
            { 
                return EventType::SHIP_DESTROYED; 
            }
    };

    class EventDispatcher {
        using EventListener = std::function<void(Event&)>;
        public:
            static void AddEventListener(EventType type, EventListener listener)
            {
                listeners[type].push_back(listener);
            }

            static void DispatchEvent(Event& event) 
            {
                for (auto &&i : listeners[event.GetType()])
                {
                    i(event);
                }
                
            }
    
        private:
            static EventDispatcher _instance;
            inline static std::map<EventType, std::vector<EventListener>> listeners;
    };
} // namespace Asteroids



#endif
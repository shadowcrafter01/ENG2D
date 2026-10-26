#ifndef ENG_ACTION_HPP
#define ENG_ACTION_HPP

#include <SDL3/SDL.h>
#include <vector>
#include <functional>

namespace ENG
{
    class Action
    {
    private:
        static std::vector<Action *> _instances;

        void _Update()
        {
            for (auto f : _functions)
            {
                f();
            }
        }

        std::vector<std::function<void()>> _functions;
        std::vector<SDL_Scancode> _keys;

        const char *_name;

    public:
        Action(const char *name)
        {
            _name = name;
            _instances.push_back(this);
        }
        Action(const char *name, std::function<void()> f)
        {
            _name = name;
            _functions.push_back(f);
            _instances.push_back(this);
        }
        Action(const char *name, SDL_Scancode key, std::function<void()> f)
        {
            _name = name;
            _keys.push_back(key);
            _functions.push_back(f);
            _instances.push_back(this);
        }

        void AssignFunction(std::function<void()> f)
        {
            _functions.push_back(f);
        }

        void AssignKey(SDL_Scancode sc)
        {
            _keys.push_back(sc);
        }

        static void UpdateAll(const SDL_Event &event)
        {

            switch (event.type)
            {
            case SDL_EVENT_KEY_DOWN:
                if (!event.key.repeat)
                {
                    
                }
                break;
            case SDL_EVENT_KEY_UP:

                break;
            default:
                break;
            }

            for (auto a : _instances)
            {
                a->_Update();
            }
        }
    };
}

#endif
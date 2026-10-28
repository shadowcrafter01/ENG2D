#ifndef ENG_ACTION_HPP
#define ENG_ACTION_HPP

#include <SDL3/SDL.h>
#include <vector>
#include <functional>
#include "Input.hpp"

namespace ENG
{
    class Action
    {
    private:
        static std::vector<Action *> _instances;

        void _Update(std::unordered_set<SDL_Scancode> keys)
        {
            _state = false;
            for (auto k : _keys)
            {
                if (!_state && keys.count(k) > 0)
                {
                    _state = true;
                }
                for (auto f : _functions)
                {
                    f(_state);
                }
            }
        }

        std::vector<std::function<void(bool)>> _functions;

        std::vector<SDL_Scancode> _keys;

        bool _state = false;

    public:
        Action()
        {
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f)
        {
            _functions.push_back(f);
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f, SDL_Scancode key)
        {
            _keys.push_back(key);
            _functions.push_back(f);
            _instances.push_back(this);
        }

        void AssignFunction(std::function<void(bool)> f)
        {
            _functions.push_back(f);
        }
        void AssignKey(SDL_Scancode sc)
        {
            _keys.push_back(sc);
        }

        bool GetState()
        {
            return _state;
        }

        static void UpdateAll()
        {
            std::unordered_set<SDL_Scancode> keys = Input::Keyboard::GetKeySet();

            for (auto a : _instances)
            {
                a->_Update(keys);
            }
        }
    };
}

#endif
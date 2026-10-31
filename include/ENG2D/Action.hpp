#ifndef ENG_ACTION_HPP
#define ENG_ACTION_HPP

#include <SDL3/SDL.h>
#include <vector>
#include <functional>
#include <optional>
#include "Input.hpp"
#include "Dingus.hpp"

namespace ENG
{
    class Action
    {
    private:
        inline static std::vector<Action *> _instances = {};

        void _Update(std::unordered_set<SDL_Scancode> keys, std::unordered_set<Uint8> buttons)
        {
            _state = false;
            for (auto in : _inputs)
            {
                if (in.isMouseInput)
                {
                    if (buttons.count(in.button) > 0)
                    {
                        _state = true;
                        break;
                    }
                }
                else
                {
                    if (keys.count(in.key) > 0)
                    {
                        _state = true;
                        break;
                    }
                }
            }
            for (auto f : _functions)
            {
                f(_state);
            }
        }

        std::vector<std::function<void(bool)>> _functions;

        struct InputType
        {
            InputType(SDL_Scancode keyboardKey)
            {
                isMouseInput = false;
                key = keyboardKey;
            }
            InputType(Uint8 mouseButton)
            {
                isMouseInput = true;
                button = mouseButton;
            }
            bool isMouseInput;
            SDL_Scancode key;
            Uint8 button;
        };

        std::vector<InputType> _inputs;

        bool _state = false;
        bool _active = true;

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
            InputType newInput(key);
            _inputs.push_back(newInput);
            _functions.push_back(f);
            _instances.push_back(this);
        }
        Action(std::function<void(bool)> f, Uint8 button)
        {
            InputType newInput(button);
            _inputs.push_back(button);
            _functions.push_back(f);
            _instances.push_back(this);
        }

        void AssignFunction(std::function<void(bool)> f)
        {
            _functions.push_back(f);
        }
        void AssignKey(SDL_Scancode sc)
        {
            _inputs.push_back(sc);
        }

        bool GetState()
        {
            return _state;
        }

        Action *SetActive(bool state)
        {
            _active = state;
            return this;
        }

        static void UpdateAll()
        {
            std::unordered_set<SDL_Scancode> keys = Input::Keyboard::GetKeySet();
            std::unordered_set<Uint8> buttons = Input::Mouse::GetButtonSet();

            for (auto a : _instances)
            {
                if (a->_active)
                {
                    a->_Update(keys, buttons);
                }
            }
        }
    };
}

#endif
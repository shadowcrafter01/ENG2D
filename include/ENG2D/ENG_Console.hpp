#ifndef ENG_CONSOLE_HPP
#define ENG_CONSOLE_HPP

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <ENG_Timer.hpp>

class ENG_Console
{
private:
    static std::string reportCurrentMS()
    {
        std::string time = std::to_string(std::round(timer->now_ms() * 100) / 100);
        std::string buffer = "";
        int decimal = 0;
        for (char i : time)
        {
            if (i == '.' || decimal > 0)
                decimal++;
            if (decimal > 3)
                break;
            buffer = buffer + i;
        }

        for (int i = buffer.length(); i < 8; ++i)
        {
            buffer = " " + buffer;
        }

        return "[" + buffer + "]";
    }

    inline static ENG_Timer *timer;
    inline static double timeStore;

public:
    ENG_Console(ENG_Timer *timer_in)
    {
        timer = timer_in;
    }

    static void Log(std::string message, std::string label = "")
    {
        std::cout << label << message << "\n";
    }

    static void LogInfo(std::string message, std::string label_override = " -INFO : ")
    {
        std::cout << reportCurrentMS() << label_override << message << "\n";
    }

    static void LogDebug(double message, std::string label_override = " -DEBUG: ")
    {
        std::cout << reportCurrentMS() << label_override << message << "\n";
    }

    static void LogWarn(std::string message)
    {
        std::cout << reportCurrentMS() << " -WARN : " << message << "\n";
    }

    static void LogError(std::string message = "", std::string error = SDL_GetError())
    {
        std::cout << reportCurrentMS() << " -ERROR: " << message << " -> " << error << "\n";
    }

    static void LogLoadStart(std::string message)
    {
        std::cout << reportCurrentMS() << " -LOAD : " << message << "... ";
        timeStore = timer->now_ms();
    }
    static void LogLoadEnd(bool success, std::string follow_up = "")
    {
        if (success)
        {
            if (follow_up == "")
            {
                std::cout << "Success in " << std::to_string(timer->now_ms() - timeStore) << "ms\n";
            }
            else
            {
                std::cout << "Success in " << std::to_string(timer->now_ms() - timeStore) << "ms -> " << follow_up << "\n";
            }
        }
        else
        {
            if (follow_up == "")
            {
                std::cout << "Failed in " << std::to_string(timer->now_ms() - timeStore) << "ms -> " << SDL_GetError() << "\n";
            }
            else
            {
                std::cout << "Failed in " << std::to_string(timer->now_ms() - timeStore) << "ms -> " << follow_up << " -> " << SDL_GetError() << "\n";
            }
        }
    }
};
// static ENG_Console console;

#endif
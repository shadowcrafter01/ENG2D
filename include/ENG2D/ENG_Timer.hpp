#ifndef TIMER_HPP
#define TIMER_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include <thread>
#include <iostream>
#include <iomanip>
#include <stdexcept>


class ENG_Timer
{
public:
    ENG_Timer(double target_fps = 1000) : _running(true),
                                          _currentNs(0),
                                          targetFPS(target_fps),
                                          FPS(0)
    {
        _thread = std::thread(&ENG_Timer::run, this);
    }

    ~ENG_Timer()
    {
        stop();
    }

    // Non-copyable, non-movable
    ENG_Timer(const ENG_Timer &) = delete;
    ENG_Timer &operator=(const ENG_Timer &) = delete;

    // Returns the latest captured nanosecond count since epoch of steady_clock
    double now_ns() const noexcept
    {
        return _currentNs.load(std::memory_order_acquire);
    }

    double now_ms() const noexcept
    {
        return _currentNs.load(std::memory_order_acquire) / 1000000.0f;
    }

    double now_s() const noexcept
    {
        return _currentNs.load(std::memory_order_acquire) / 1000000000.0f;
    }

    void update()
    {
        double now = now_ns();
        delta = (now - timeLast) / 1000000000.0f;
        physicsDelta = delta * targetFPS;
        if (physicsDelta > 1)
            physicsDelta = 1;
        timeLast = now;
        if (delta == 0)
        {
            FPS = 0;
        }
        else
        {
            FPS = (0.5 * FPS) + ((1 / delta) * (1 - 0.5));
        }
    }
    double physicsDelta;
    double delta;
    double FPS;
    double targetFPS;

    // Stop the updating thread
    void stop()
    {
        bool expected = true;
        if (_running.compare_exchange_strong(expected, false))
        {
            if (_thread.joinable())
            {
                _thread.join();
            }
        }
    }

private:
    void run()
    {
        // Capture the steady_clock epoch once, then measure offsets
        const auto start = std::chrono::steady_clock::now();

        while (_running.load(std::memory_order_acquire))
        {
            auto elapsed = std::chrono::steady_clock::now() - start;
            uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            _currentNs.store(ns, std::memory_order_release);
        }
    }

    double timeLast = 0;
    std::atomic<bool> _running;
    std::atomic<uint64_t> _currentNs;
    std::thread _thread;
};

#endif
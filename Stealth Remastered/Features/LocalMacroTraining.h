#pragma once
#include <cstdint>

namespace FlavelahTraining
{
struct KeyEvent
{
    char key = 0;
    bool down = false;
};

inline int WeaponIndex(int id)
{
    switch (id)
    {
        case 24: return 0;
        case 27: return 1;
        case 31: return 2;
        case 30: return 3;
        case 29: return 4;
        default: return -1;
    }
}

// Explicit opt-in to the local test environment. All other addresses fail closed.
inline bool IsLoopback(const char* address)
{
    constexpr char local[] = "127.0.0.1";
    if (!address) return false;
    for (unsigned i = 0; i < sizeof(local); ++i)
        if (address[i] != local[i]) return false;
    return true;
}

class Scheduler
{
public:
    int index() const { return selected_; }
    bool running() const { return running_; }
    bool armed() const { return armed_; }
    bool completed() const { return completed_; }

    KeyEvent cancel()
    {
        KeyEvent event{ held_, false };
        held_ = 0;
        running_ = armed_ = completed_ = false;
        selected_ = -1;
        step_ = 0;
        return event;
    }

    KeyEvent tick(std::uint64_t now, bool allowed, bool aim, bool fire,
                  int weapon, const bool (&enabled)[5])
    {
        completed_ = false;
        const int idx = WeaponIndex(weapon);
        if (!allowed) return cancel();
        if (running_)
        {
            if (!enabled[selected_] || aim || fire) return cancel();
            if (now < next_) return {};
            const char* sequence = selected_ == 0 ? "ECQE" : "QCEQ";
            const char key = sequence[step_ / 2];
            const bool down = step_ % 2 == 0;
            held_ = down ? key : 0;
            ++step_;
            next_ = now + 40; // ms; no blocking sleeps or multi-event catch-up.
            if (step_ == 8)
            {
                running_ = false;
                selected_ = -1;
                completed_ = true;
            }
            return {key, down};
        }

        if (idx < 0 || !enabled[idx]) return cancel();
        if (aim && fire)
        {
            selected_ = idx;
            armed_ = true;
            return {};
        }
        if (armed_ && selected_ != idx) return cancel();
        if (armed_ && !aim && !fire)
        {
            armed_ = false;
            running_ = true;
            step_ = 0;
            next_ = now + 40;
        }
        return {};
    }

private:
    int selected_ = -1;
    unsigned step_ = 0;
    std::uint64_t next_ = 0;
    char held_ = 0;
    bool armed_ = false;
    bool running_ = false;
    bool completed_ = false;
};
}

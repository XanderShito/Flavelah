#include "Features/LocalMacroTraining.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace FlavelahTraining;

int main()
{
    assert(IsLoopback("127.0.0.1"));
    assert(!IsLoopback("127.0.0.10"));
    assert(!IsLoopback("127.0.0.1.example"));
    assert(!IsLoopback("192.168.1.2"));
    assert(!IsLoopback(""));
    assert(!IsLoopback(nullptr));
    bool all[5] = { true, true, true, true, true };
    bool off[5] = {};
    const int weapons[] = {24, 27, 31, 30, 29};
    for (int w : weapons)
    {
        Scheduler s;
        assert(!s.tick(0, true, true, true, w, off).key);
        assert(!s.armed());
        assert(!s.tick(1, false, true, true, w, all).key);
        assert(!s.armed());
        assert(!s.tick(2, true, true, true, w, all).key);
        assert(s.armed());
        assert(!s.tick(3, true, true, false, w, all).key);
        assert(s.armed());
        assert(!s.tick(4, true, false, false, w, all).key);
        assert(s.running());
        assert(!s.tick(43, true, false, false, w, all).key);
        std::string keys;
        for (unsigned i = 0; i < 8; ++i)
        {
            // Weapon selection can change as a result of the sequence.
            const auto event = s.tick(44 + i * 40, true, false, false, 0, all);
            assert(event.key);
            assert(event.down == (i % 2 == 0));
            if (event.down) keys += event.key;
            else assert(keys.back() == event.key);
        }
        assert(keys == (w == 24 ? "ECQE" : "QCEQ"));
        assert(s.completed() && !s.running());
    }
    Scheduler cancel;
    cancel.tick(0, true, true, true, 24, all);
    cancel.tick(1, true, false, false, 24, all);
    auto down = cancel.tick(41, true, false, false, 24, all);
    assert(down.key && down.down);
    auto up = cancel.tick(42, false, false, false, 24, all);
    assert(up.key == down.key && !up.down);
    assert(!cancel.running() && !cancel.armed());
    Scheduler change;
    change.tick(0, true, true, true, 24, all);
    change.tick(1, true, false, false, 27, all);
    assert(!change.running());
    std::cout << "PASS: five sequences, default-off, local gate, timing, cancellation and weapon change\n";
}

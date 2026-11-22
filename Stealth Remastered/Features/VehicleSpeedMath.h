#pragma once
#include <algorithm>
#include <cmath>

namespace FlavelahVehicleSpeed
{
    // Matches SpeedHack(1).cs: scaled speed plus a fixed offset of 2.
    // Backward mode is a negative signed speed, as in CLEO opcode 04BA.
    inline float Target(float absoluteSpeed, float factor, bool forward)
    {
        if (!std::isfinite(absoluteSpeed) || absoluteSpeed < 0.f)
            return 0.f;
        factor = std::clamp(factor, 1.f, 3.f);
        const float target = absoluteSpeed * factor + 2.f;
        if (!std::isfinite(target)) return 0.f;
        // Safety limit against runaway acceleration in the physics engine.
        const float capped = std::min(target, 75.f);
        return forward ? capped : -capped;
    }

    inline bool IsModifierInput(bool modifier, bool forward, bool reverse)
    {
        return modifier && (forward != reverse);
    }
}

#include "../Stealth Remastered/Features/VehicleSpeedMath.h"
#include <cassert>
#include <cmath>

int main()
{
    using namespace FlavelahVehicleSpeed;
    assert(std::fabs(Target(10.0f, 1.2f, true) - 14.0f) < 0.0001f);
    assert(std::fabs(Target(10.0f, 1.2f, false) + 14.0f) < 0.0001f);
    assert(std::fabs(Target(10.0f, 0.2f, true) - 12.0f) < 0.0001f);
    assert(std::fabs(Target(10.0f, 5.0f, true) - 32.0f) < 0.0001f);
    assert(std::fabs(Target(80.0f, 3.0f, true) - 75.0f) < 0.0001f);
    assert(std::fabs(Target(80.0f, 3.0f, false) + 75.0f) < 0.0001f);
    assert(IsModifierInput(true, true, false));
    assert(IsModifierInput(true, false, true));
    assert(!IsModifierInput(true, true, true));
    assert(!IsModifierInput(false, true, false));
    assert(!IsModifierInput(true, false, false));
    return 0;
}

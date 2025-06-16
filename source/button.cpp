#include "common.hpp"
#include "button.hpp"

u32 kDown;
u32 kHeld;

void UpdateButtons()
{
    hidScanInput();
    kDown = hidKeysDown();
    kHeld = hidKeysHeld();
}

bool ButtonPressed(u32 keyBit)
{
    return (kDown & keyBit);
}

bool ButtonHeld(u32 keyBit)
{
    return (kHeld & keyBit);
}
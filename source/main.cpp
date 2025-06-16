// by okawaffles - 2025

#include "common.hpp"
#include "button.hpp"
#include "scan.hpp"

static u64 MIRAI_EU = 0x0004000000148900;
static u64 MIRAI_US = 0x0004000000148C00;
static u64 MIRAI_JP = 0x000400000014A800;

int main()
{
    srvInit();
    cfguInit();
    aptInit();
    hidInit();

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    printf("MiraiModConfig v1.0.0 by okawaffles\nStarting scan...\n");
    ScanForMirai();
    printf("Scan complete!");

    while (aptMainLoop())
    {
        

        UpdateButtons();
        if (ButtonPressed(KEY_START)) break;
        if (ButtonPressed(KEY_X)) {
            aptSetChainloader(MIRAI_US, MEDIATYPE_SD);
            break;
        }
    }

    srvExit();
    cfguExit();
    hidExit();
    aptExit();

    gfxExit();

    return 0;
}
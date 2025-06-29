// by okawaffles - 2025
#include "common.hpp"
#include "button.hpp"
#include "scan.hpp"
#include "parser/mesy.hpp"
#include "listing.hpp"

static const u64 MIRAI_EU = 0x0004000000148900;
static const u64 MIRAI_US = 0x0004000000148C00;
static const u64 MIRAI_JP = 0x000400000014A800;

#define DEBUG

int main()
{
    srvInit();
    cfguInit();
    aptInit();
    hidInit();
    gfxInitDefault();
    fsInit();
    consoleInit(GFX_TOP, NULL);

    Result rc = romfsInit();
    if (rc) printf("romfs init error!! rc=%ld\n", rc);
    else printf("romfs init OK!\n \n");

    printf("MiraiModConfig v1.0.0 by okawaffles\nLoading, please wait...\n \n");

#ifdef DEBUG
    printf("Starting scan...\n");
    ScanForMirai();
    printf("Scan complete!\n \n");

    printf("Testing MESY...\n");
    int success = MESY::LoadMESY("romfs:/mirai/resource.mesy");
    std::string value;
    printf("MESY load result code: %d\n", success);
    if (success == 0) 
    {
        success = MESY::GetValueByKeyName("MESY_LOADER_TEST", &value);
        printf("MESY prop read test: %d\n", success);
        if (success == 0)
        {
            printf(value.c_str());
            printf("\n");
        }
        else
        {
            printf("MESY prop read error: ");
            printf(MESY::GetLastError());
            printf("\n");
        }
    } 
    else
    {
        printf("MESY failure: ");
        printf(MESY::GetLastError());
        printf("\n");
    }
#else
    ScanForMirai();
#endif
    
    sleep(1);
    
    u64 launchId = MIRAI_US;
    FS_MediaType launchContext = MEDIATYPE_SD;
    
#ifndef DEBUG
    consoleClear();
#endif
    
    printf("Press X to launch Project Mirai DX.\nPress Y to cycle options.\nPress B to list charts.\n \n");
    printf("Currently Selected: USA on SD_CARD\n");

    bool updateDisplay = false;

    while (aptMainLoop())
    {
        if (updateDisplay)
        {
            if (!(ButtonPressed(KEY_R) || ButtonHeld(KEY_R))) consoleClear(); // hold R to bypass refresh clear
            printf("Press X to launch Project Mirai DX.\nPress Y to cycle options.\n \n");

            std::stringstream sel;
            sel << "Currently Selected: ";

            switch (launchId)
            {
                case MIRAI_US:
                    sel << "USA on ";
                    break;

                case MIRAI_JP:
                    sel << "JPN on ";
                    break;
                    
                case MIRAI_EU:
                    sel << "EUR on ";
                    break;
            }

            switch (launchContext)
            {
                case MEDIATYPE_SD:
                    sel << "SD CARD";
                    break;

                case MEDIATYPE_NAND:
                    sel << "NAND";
                    break;
                    
                case MEDIATYPE_GAME_CARD:
                    sel << "GAME CARD";
                    break;
            }

            sel << std::endl;
            std::cout << sel.str();

            updateDisplay = false;
        }

        UpdateButtons();


        if (ButtonPressed(KEY_START)) break; // exit

        if (ButtonPressed(KEY_X)) 
        {
            aptSetChainloader(launchId, launchContext);
            break;
        }

        if (ButtonPressed(KEY_Y))
        {
            // jank way of cycling lol fix this later
            if (launchContext == MEDIATYPE_GAME_CARD)
            {
                launchId = MIRAI_US;
                launchContext = MEDIATYPE_SD;
                updateDisplay = true;
                continue;
            }

            if (launchId == MIRAI_US && launchContext == MEDIATYPE_SD) { launchId = MIRAI_EU; updateDisplay = true; continue; }
            if (launchId == MIRAI_EU && launchContext == MEDIATYPE_SD) { launchId = MIRAI_JP; updateDisplay = true; continue; }
            if (launchId == MIRAI_JP && launchContext == MEDIATYPE_SD) {
                launchId = MIRAI_US;
                launchContext = MEDIATYPE_NAND;
                updateDisplay = true; 
                continue;
            }

            if (launchId == MIRAI_US && launchContext == MEDIATYPE_NAND) { launchId = MIRAI_EU; updateDisplay = true; continue; }
            if (launchId == MIRAI_EU && launchContext == MEDIATYPE_NAND) { launchId = MIRAI_JP; updateDisplay = true; continue; }
            if (launchId == MIRAI_JP && launchContext == MEDIATYPE_NAND) {
                launchId = MIRAI_US;
                launchContext = MEDIATYPE_SD;
                updateDisplay = true; 
                continue;
            }
        }

        if (ButtonPressed(KEY_B))
        {
            size_t num_charts;
            int success = Listing::GetChartListingCount(&num_charts);

            if (success != 0)
            {
                std::cout << "Error loading charts. See above for error." << std::endl;
                continue;
            }

            // for (size_t i = 0; i < num_charts; i++)
            // {
            //     ChartMod chart;
            //     Listing::ReadChartAt(i, &chart);
            //     std::stringstream item;
            //     item << "--\n" << chart.song_name << " by " << chart.song_artist << "\nCharted by " << chart.chart_author << " | BPM: " << chart.bpm << std::endl;
            //     printf(item.str().c_str());
            // }
        }
    }

    fsExit();
    romfsExit();
    srvExit();
    cfguExit();
    hidExit();
    aptExit();
    gfxExit();

    return 0;
}
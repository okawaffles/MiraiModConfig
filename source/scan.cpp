// This file scans for Project Mirai DX in US, JP, and EU regions

#include "common.hpp"
#include "scan.hpp"

std::string VALIDITY_STRINGS[] = {"NAND", "SD Card", "Game Card"};

static u64 MIRAI_EU = 0x0004000000148900;
static u64 MIRAI_US = 0x0004000000148C00;
static u64 MIRAI_JP = 0x000400000014A800;


void ScanForMirai()
{
    amInit();

    u32 title_count_nand;
    u32 title_count_sd;
    u32 title_count_card;

    bool card_inserted;

    AM_GetTitleCount(MEDIATYPE_NAND, &title_count_nand);
    AM_GetTitleCount(MEDIATYPE_SD, &title_count_sd);
    AM_GetTitleCount(MEDIATYPE_GAME_CARD, &title_count_card);

    printf("[scan]: %lu titles on NAND\n", title_count_nand);
    printf("[scan]: %lu titles on SD\n", title_count_sd);
    printf("[scan]: %lu titles on GAME CARD\n", title_count_card);

    if (title_count_card > 1) {
        card_inserted = false;
        printf("[scan]: more than 1 title on the game card\n");
        printf("[scan]: assuming no game card inserted\n");
    }

    // allocate memory for title IDs
    u64* titleIds_NAND = (u64*)malloc(title_count_nand * sizeof(u64));
    u64* titleIds_SD = (u64*)malloc(title_count_sd * sizeof(u64));
    u64* titleIds_CARD = (u64*)malloc(title_count_card * sizeof(u64));
    AM_TitleEntry* titleInfo = (AM_TitleEntry*)malloc(title_count_nand * sizeof(AM_TitleEntry));

    // get title list
    AM_GetTitleList(&title_count_nand, MEDIATYPE_NAND, title_count_nand, titleIds_NAND);
    AM_GetTitleList(&title_count_sd, MEDIATYPE_SD, title_count_sd, titleIds_SD);
    AM_GetTitleList(&title_count_card, MEDIATYPE_GAME_CARD, title_count_card, titleIds_CARD);

    AM_GetTitleInfo(MEDIATYPE_NAND, title_count_nand, titleIds_NAND, titleInfo);
    AM_GetTitleInfo(MEDIATYPE_SD, title_count_sd, titleIds_SD, titleInfo);
    AM_GetTitleInfo(MEDIATYPE_GAME_CARD, title_count_card, titleIds_CARD, titleInfo);

    printf(" \nScanning NAND...\n");

    // print title IDs
    u8 usa = 0;
    u8 jpn = 0;
    u8 eur = 0;
    for (u32 i = 0; i < title_count_nand; i++) {
        // printf("Title %lu: 0x%016llX\n", i, titleIds_NAND[i]);
        if (titleIds_NAND[i] == MIRAI_US) usa = 1;
        if (titleIds_NAND[i] == MIRAI_JP) jpn = 1;
        if (titleIds_NAND[i] == MIRAI_EU) eur = 1;
    }

    printf(" \nScanning SD...\n");

    // print title IDs
    for (u32 i = 0; i < title_count_sd; i++) {
        // printf("Title %lu: 0x%016llX\n", i, titleIds_NAND[i]);
        if (titleIds_SD[i] == MIRAI_US) usa = 2;
        if (titleIds_SD[i] == MIRAI_JP) jpn = 2;
        if (titleIds_SD[i] == MIRAI_EU) eur = 2;
    }

    printf(" \nScanning GAME_CARD...\n");

    // print title IDs
    if (card_inserted) for (u32 i = 0; i < title_count_card; i++) {
        // printf("Title %lu: 0x%016llX\n", i, titleIds_NAND[i]);
        if (titleIds_CARD[i] == MIRAI_US) usa = 3;
        if (titleIds_CARD[i] == MIRAI_JP) jpn = 3;
        if (titleIds_CARD[i] == MIRAI_EU) eur = 3;
    }

    if (usa > 0) printf("\x1b[32mUSA - %s\x1b[0m\n", VALIDITY_STRINGS[usa - 1].c_str());
    if (jpn > 0) printf("\x1b[32mJPN - %s\x1b[0m\n", VALIDITY_STRINGS[jpn - 1].c_str());
    if (eur > 0) printf("\x1b[32mEUR - %s\x1b[0m\n", VALIDITY_STRINGS[eur - 1].c_str());

    amExit();
}
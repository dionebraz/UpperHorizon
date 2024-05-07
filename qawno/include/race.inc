/*
	Racing System 1.0 by Raf.

	Changelog:
    Version 1.0 (17/07/2017 - 21:00)
    - Initial Release
*/

#include <sscanf2>
#include <foreach>
#include <zcmd>
#include <notify>

#define MAX_RACES 			3
#define MAX_CHECKS 			40

#define DIALOG_UNUSED 		0
#define DIALOG_RACE 		1

#define RACE_VIRTUALWORLD 	1

enum RACE_playerInfo
{
    RACE_playerUsername[MAX_PLAYER_NAME],

    RACE_playerVehicle,

    RACE_playerPosition,
    RACE_playerCheckpointsPassed,
    RACE_playerTimeTookToFinish,

    Text:RACE_textdrawBar,
    Text:RACE_textdrawBox,
    Text:RACE_textdrawTitle,
    Text:RACE_textdrawName,
    Text:RACE_textdrawPosition,
    Text:RACE_textdrawCheckpoint,
    Text:RACE_textdrawTimeLeft,
    Text:RACE_textdrawSpeed,
    Text:RACE_textdrawCircleRed,
    Text:RACE_textdrawCircleYellow,
    Text:RACE_textdrawCircleGreen,
    Text:RACE_textdrawSecondsLeft,
    Text:RACE_textdrawStartIn,

    bool:RACE_isPlayerInRace,

    Text:RACE_textdrawTip,
    Text:RACE_textdrawTip2,

    Text:RACE_textdrawUsebox[24],
    Text:RACE_textdrawRaceHud[22]
};

enum RACE_raceInfo
{
    raceName[24],

    raceVehicleID,
    raceMaxCheckpoints,
    raceInterior,
    raceDistance,
    raceRecord,
    raceRecordPlaced,
    raceRecordHolder[24],

    Float:raceSpawnPositionX,
    Float:raceSpawnPositionY,
    Float:raceSpawnPositionZ,
    Float:raceSpawnPositionA,

    Float:raceCheckpointPositionX,
    Float:raceCheckpointPositionY,
    Float:raceCheckpointPositionZ
};

new RACE_pInfo[MAX_PLAYERS][RACE_playerInfo],

    RACE_rInfo[MAX_RACES][MAX_CHECKS][RACE_raceInfo],

    RACE_loadedRaces = 0,
    RACE_runningID,

    RACE_playersInEvent = 0,
    RACE_playersLeft = 0,
    RACE_playersSpawned = 0,

    RACE_timeLeft,
    RACE_timerCounter,

    RACE_countdownTimer,
    RACE_checkRaceTimer,

    RACE_checkpoints[MAX_CHECKS],

    RACE_spawnSlots[16] = {-1, ...},

                          bool:RACE_isRaceStarted = false,
                          bool:RACE_isRaceInLobby = false,

                          RACE_globalString[144];

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    if (!success)
    {
        notification.Show(playerid, "Erro!", "Comando inexistente.", "ld_chat:badchat");
    }
    return 1;
}

RACE_loadTextdraws()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        // TIPS - TEST MESSAGE

        RACE_pInfo[i][RACE_textdrawTip] = TextDrawCreate(143.367523, 359.916778, "Test message");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawTip], 0.283672, 1.570829);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawTip], 1);
        TextDrawColor(RACE_pInfo[i][RACE_textdrawTip], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawTip], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawTip], 1);
        TextDrawBackgroundColor(RACE_pInfo[i][RACE_textdrawTip], 51);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawTip], 1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawTip], 1);

        RACE_pInfo[i][RACE_textdrawTip2] = TextDrawCreate(143.367523, 359.916778, "Test message");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawTip2], 0.283672, 1.570829);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawTip2], 1);
        TextDrawColor(RACE_pInfo[i][RACE_textdrawTip2], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawTip2], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawTip2], 1);
        TextDrawBackgroundColor(RACE_pInfo[i][RACE_textdrawTip2], 51);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawTip2], 1);

        // SEMAFORO TEXTDRAW COM CONTAGEM

        RACE_pInfo[i][RACE_textdrawUsebox][0] = TextDrawCreate(260.000, 21.750, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][0], 120.000, 18.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][0], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][0], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][0], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][0], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][0], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][0], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][0], true);

        RACE_pInfo[i][RACE_textdrawUsebox][1] = TextDrawCreate(246.000, 17.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][1], 30.000, 30.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][1], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][1], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][1], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][1], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][1], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][1], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][1], true);

        RACE_pInfo[i][RACE_textdrawUsebox][2] = TextDrawCreate(365.000, 17.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][2], 30.000, 30.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][2], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][2], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][2], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][2], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][2], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][2], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][2], true);

        RACE_pInfo[i][RACE_textdrawUsebox][3] = TextDrawCreate(260.000, 73.750, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][3], 120.000, 18.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][3], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][3], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][3], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][3], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][3], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][3], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][3], true);

        RACE_pInfo[i][RACE_textdrawUsebox][4] = TextDrawCreate(246.000, 67.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][4], 30.000, 30.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][4], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][4], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][4], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][4], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][4], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][4], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][4], true);

        RACE_pInfo[i][RACE_textdrawUsebox][5] = TextDrawCreate(365.000, 67.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][5], 30.000, 30.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][5], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][5], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][5], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][5], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][5], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][5], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][5], true);

        RACE_pInfo[i][RACE_textdrawUsebox][6] = TextDrawCreate(251.000, 32.750, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][6], 138.850, 50.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][6], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][6], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][6], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][6], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][6], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][6], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][6], true);

        RACE_pInfo[i][RACE_textdrawUsebox][7] = TextDrawCreate(261.000, 26.700, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][7], 118.000, 10.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][7], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][7], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][7], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][7], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][7], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][7], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][7], true);

        RACE_pInfo[i][RACE_textdrawUsebox][8] = TextDrawCreate(253.000, 24.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][8], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][8], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][8], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][8], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][8], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][8], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][8], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][8], true);

        RACE_pInfo[i][RACE_textdrawUsebox][9] = TextDrawCreate(372.000, 24.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][9], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][9], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][9], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][9], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][9], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][9], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][9], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][9], true);

        RACE_pInfo[i][RACE_textdrawUsebox][10] = TextDrawCreate(262.000, 77.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][10], 118.000, 10.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][10], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][10], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][10], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][10], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][10], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][10], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][10], true);

        RACE_pInfo[i][RACE_textdrawUsebox][11] = TextDrawCreate(253.000, 74.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][11], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][11], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][11], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][11], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][11], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][11], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][11], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][11], true);

        RACE_pInfo[i][RACE_textdrawUsebox][12] = TextDrawCreate(372.000, 74.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][12], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][12], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][12], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][12], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][12], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][12], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][12], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][12], true);

        RACE_pInfo[i][RACE_textdrawUsebox][13] = TextDrawCreate(255.500, 32.750, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][13], 129.600, 50.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][13], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][13], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][13], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][13], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][13], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][13], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][13], true);

        RACE_pInfo[i][RACE_textdrawUsebox][14] = TextDrawCreate(261.000, 28.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][14], 118.000, 5.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][14], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][14], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][14], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][14], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][14], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][14], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][14], true);

        RACE_pInfo[i][RACE_textdrawUsebox][15] = TextDrawCreate(256.000, 27.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][15], 9.000, 9.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][15], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][15], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][15], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][15], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][15], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][15], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][15], true);

        RACE_pInfo[i][RACE_textdrawUsebox][16] = TextDrawCreate(375.000, 27.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][16], 9.000, 9.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][16], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][16], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][16], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][16], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][16], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][16], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][16], true);

        RACE_pInfo[i][RACE_textdrawUsebox][17] = TextDrawCreate(260.000, 79.500, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][17], 121.000, 5.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][17], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][17], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][17], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][17], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][17], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][17], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][17], true);

        RACE_pInfo[i][RACE_textdrawUsebox][18] = TextDrawCreate(256.000, 77.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][18], 9.000, 9.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][18], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][18], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][18], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][18], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][18], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][18], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][18], true);

        RACE_pInfo[i][RACE_textdrawUsebox][19] = TextDrawCreate(375.000, 77.000, "LD_BEAT:chit");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][19], 9.000, 9.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][19], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][19], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][19], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][19], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][19], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][19], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][19], true);

        RACE_pInfo[i][RACE_textdrawUsebox][20] = TextDrawCreate(257.500, 32.750, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][20], 125.000, 48.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][20], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][20], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][20], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][20], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][20], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][20], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][20], true);

        RACE_pInfo[i][RACE_textdrawCircleRed] = TextDrawCreate(260.000, 45.000, "LD_POOL:ball");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawCircleRed], 25.000, 25.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawCircleRed], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleRed], -16777171);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawCircleRed], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawCircleRed], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawCircleRed], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawCircleRed], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawCircleRed], true);

        RACE_pInfo[i][RACE_textdrawCircleYellow] = TextDrawCreate(288.000, 45.000, "LD_POOL:ball");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawCircleYellow], 25.000, 25.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawCircleYellow], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleYellow], -65491);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawCircleYellow], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawCircleYellow], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawCircleYellow], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawCircleYellow], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawCircleYellow], true);

        RACE_pInfo[i][RACE_textdrawCircleGreen] = TextDrawCreate(316.000, 45.000, "LD_POOL:ball");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawCircleGreen], 25.000, 25.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawCircleGreen], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleGreen], 16711725);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawCircleGreen], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawCircleGreen], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawCircleGreen], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawCircleGreen], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawCircleGreen], true);

        RACE_pInfo[i][RACE_textdrawUsebox][21] = TextDrawCreate(343.000, 40.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][21], 37.000, 35.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][21], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][21], 808464639);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][21], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][21], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][21], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][21], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][21], true);

        RACE_pInfo[i][RACE_textdrawSecondsLeft] = TextDrawCreate(361.000, 57.000, "- 30s -");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawSecondsLeft], 0.300, 1.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawSecondsLeft], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawSecondsLeft], -101);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawSecondsLeft], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawSecondsLeft], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawSecondsLeft], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawSecondsLeft], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawSecondsLeft], true);

        RACE_pInfo[i][RACE_textdrawStartIn] = TextDrawCreate(362.000, 45.000, "Iniciando em");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawStartIn], 0.200, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawStartIn], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawStartIn], -101);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawStartIn], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawStartIn], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawStartIn], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawStartIn], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawStartIn], true);

        RACE_pInfo[i][RACE_textdrawUsebox][22] = TextDrawCreate(263.000, -3.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][22], 4.000, 25.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][22], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][22], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][22], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][22], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][22], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][22], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][22], true);

        RACE_pInfo[i][RACE_textdrawUsebox][23] = TextDrawCreate(372.000, -3.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawUsebox][23], 4.000, 25.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawUsebox][23], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawUsebox][23], 437918463);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawUsebox][23], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawUsebox][23], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawUsebox][23], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawUsebox][23], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawUsebox][23], true);

        // HUD COM INFORMAÇÕES DA CORRIDA

        RACE_pInfo[i][RACE_textdrawRaceHud][0] = TextDrawCreate(30.000, 10.000, "0/0");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][0], 0.750, 3.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][0], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][0], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][0], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][0], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][0], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][0], TEXT_DRAW_FONT_2);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][0], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][1] = TextDrawCreate(524.000, 234.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][1], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][1], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][1], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][1], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][1], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][1], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][1], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][1], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][2] = TextDrawCreate(540.000, 234.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][2], 70.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][2], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][2], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][2], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][2], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][2], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][2], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][2], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][3] = TextDrawCreate(532.000, 235.000, "4");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][3], 0.349, 1.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][3], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][3], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][3], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][3], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][3], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][3], TEXT_DRAW_FONT_3);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][3], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][4] = TextDrawCreate(544.000, 236.000, "Nome_Sobrenome");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][4], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][4], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][4], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][4], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][4], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][4], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][4], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][4], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][5] = TextDrawCreate(524.000, 174.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][5], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][5], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][5], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][5], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][5], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][5], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][5], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][5], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][6] = TextDrawCreate(540.000, 174.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][6], 70.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][6], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][6], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][6], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][6], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][6], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][6], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][6], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][7] = TextDrawCreate(532.000, 175.000, "1");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][7], 0.349, 1.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][7], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][7], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][7], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][7], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][7], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][7], TEXT_DRAW_FONT_3);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][7], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][8] = TextDrawCreate(544.000, 176.000, "Nome_Sobrenome");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][8], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][8], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][8], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][8], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][8], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][8], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][8], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][8], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][9] = TextDrawCreate(524.000, 194.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][9], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][9], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][9], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][9], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][9], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][9], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][9], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][9], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][10] = TextDrawCreate(540.000, 194.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][10], 70.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][10], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][10], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][10], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][10], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][10], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][10], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][10], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][11] = TextDrawCreate(532.000, 195.000, "2");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][11], 0.349, 1.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][11], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][11], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][11], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][11], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][11], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][11], TEXT_DRAW_FONT_3);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][11], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][12] = TextDrawCreate(544.000, 196.000, "Nome_Sobrenome");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][12], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][12], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][12], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][12], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][12], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][12], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][12], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][12], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][13] = TextDrawCreate(524.000, 214.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][13], 16.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][13], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][13], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][13], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][13], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][13], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][13], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][13], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][14] = TextDrawCreate(540.000, 214.000, "LD_SPAC:white");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][14], 70.000, 16.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][14], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][14], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][14], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][14], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][14], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][14], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][14], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][15] = TextDrawCreate(532.000, 215.000, "3");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][15], 0.349, 1.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][15], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][15], 437918408);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][15], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][15], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][15], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][15], TEXT_DRAW_FONT_3);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][15], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][16] = TextDrawCreate(544.000, 216.000, "Nome_Sobrenome");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][16], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][16], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][16], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][16], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][16], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][16], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][16], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][16], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][17] = TextDrawCreate(567.000, 140.000, "1st");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][17], 0.750, 3.500);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][17], TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][17], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][17], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][17], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][17], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][17], TEXT_DRAW_FONT_2);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][17], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][18] = TextDrawCreate(45.000, 51.000, "5:00");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][18], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][18], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][18], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][18], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][18], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][18], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][18], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][18], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][19] = TextDrawCreate(30.000, 70.000, "HUD:radar_Flag");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][19], 10.000, 10.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][19], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][19], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][19], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][19], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][19], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][19], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][19], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][20] = TextDrawCreate(44.000, 69.000, "Corrida");
        TextDrawLetterSize(RACE_pInfo[i][RACE_textdrawRaceHud][20], 0.250, 1.200);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][20], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][20], -56);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][20], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][20], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][20], 150);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][20], TEXT_DRAW_FONT_1);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][20], true);

        RACE_pInfo[i][RACE_textdrawRaceHud][21] = TextDrawCreate(29.000, 50.000, "LD_GRAV:timer");
        TextDrawTextSize(RACE_pInfo[i][RACE_textdrawRaceHud][21], 12.000, 12.000);
        TextDrawAlignment(RACE_pInfo[i][RACE_textdrawRaceHud][21], TEXT_DRAW_ALIGN_LEFT);
        TextDrawColour(RACE_pInfo[i][RACE_textdrawRaceHud][21], -1);
        TextDrawSetShadow(RACE_pInfo[i][RACE_textdrawRaceHud][21], 0);
        TextDrawSetOutline(RACE_pInfo[i][RACE_textdrawRaceHud][21], 0);
        TextDrawBackgroundColour(RACE_pInfo[i][RACE_textdrawRaceHud][21], 255);
        TextDrawFont(RACE_pInfo[i][RACE_textdrawRaceHud][21], TEXT_DRAW_FONT_SPRITE_DRAW);
        TextDrawSetProportional(RACE_pInfo[i][RACE_textdrawRaceHud][21], true);
    }
}

RACE_resetPlayerVariables(playerid)
{
    RACE_pInfo[playerid][RACE_playerPosition] = 0,
            RACE_pInfo[playerid][RACE_playerCheckpointsPassed] = 0,
                    RACE_pInfo[playerid][RACE_playerTimeTookToFinish] = 0,

                            RACE_pInfo[playerid][RACE_isPlayerInRace] = false;
}

RACE_hidePlayerRaceTextdraws(playerid)
{
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawBox]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTitle]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawName]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawPosition]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCheckpoint]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTimeLeft]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawSpeed]);
}

RACE_showPlayerSemaforoTextdraws(playerid)
{
    for (new i = 0; i < 24; i++) TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawUsebox][i]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleRed]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleYellow]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleGreen]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawSecondsLeft]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawStartIn]);
}

RACE_showPlayerRaceHudTextdraws(playerid)
{
    for (new i = 0; i < 22; i++) TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawRaceHud][i]);
}

RACE_hidePlayerRaceHudTextdraws(playerid)
{
    for (new i = 0; i < 22; i++) TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawRaceHud][i]);
}

RACE_getPlayerSpeed(playerid)
{
    new Float:X,
        Float:Y,
        Float:Z;

    if (IsPlayerInAnyVehicle(playerid))
    {
        GetVehicleVelocity(GetPlayerVehicleID(playerid), X, Y, Z);
    }
    else
    {
        GetPlayerVelocity(playerid, X, Y, Z);
    }
    return floatround(floatsqroot((X * X) + (Y * Y) + (Z * Z)) * 195.12);
}

RACE_replaceline(const filename[], const find[], const replace[])
{
    if (!fexist(filename)) return 0;

    new File:handle = fopen(filename, io_read);

    if (!handle) return 0;

    new File:tmp = ftemp();

    if (!tmp)
    {
        fclose(handle);
        return 0;
    }

    new line[256];

    while (fread(handle, line))
    {
        if (strfind(line, find) == -1)
        {
            fwrite(tmp, line);
        }
        else
        {
            fwrite(tmp, replace);
        }
    }

    fclose(handle);

    fseek(tmp, 0);
    handle = fopen(filename, io_write);

    if (!handle)
    {
        fclose(tmp);
        return 0;
    }

    while (fread(tmp, line))
    {
        fwrite(handle, line);
    }

    fclose(handle);
    fclose(tmp);
    return 1;
}

RACE_convertTime(timestamp, bool:withtime = true)
{
    new monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                        years = 1970,
                        days = 0,
                        months = 0,
                        hours = 0,
                        minutes = 0,
                        seconds = 0,
                        string[50];

    while (timestamp > 31622400)
    {
        timestamp -= 31536000;

        if (((years % 4 == 0) && (years % 100 != 0)) || (years % 400 == 0))
        {
            timestamp -= 86400;
        }

        years++;
    }

    if (((years % 4 == 0) && (years % 100 != 0)) || (years % 400 == 0))
    {
        monthDays[1] = 29;
    }
    else
    {
        monthDays[1] = 28;
    }

    while (timestamp > 86400)
    {
        timestamp -= 86400;
        days++;

        if (days == monthDays[months])
        {
            days = 0;
            months++;
        }
    }

    while (timestamp > 60)
    {
        timestamp -= 60;
        minutes++;

        if (minutes == 60)
        {
            minutes = 0;
            hours++;
        }
    }

    seconds = timestamp;

    if (withtime)
    {
        format(string, sizeof(string), "%02d/%02d/%d - %02d:%02d:%02d", days + 1, months + 1, years, hours, minutes, seconds);
    }
    else
    {
        format(string, sizeof(string), "%02d/%02d/%d", days + 1, months + 1, years);
    }
    return string;
}

RACE_loadRaceFromFile(race)
{
    new fileName[15];

    format(fileName, sizeof(fileName), "Races/%d.txt", race);

    new File:file = fopen(fileName, io_read);

    if (!file)
    {
        printf("[CORRIDA] %s Falha ao carregar.", fileName);
        return 0;
    }
    else
    {
        new line[200],

            rName[24],
            rVehicleID,
            rMaxCheckpoints,
            rInterior,
            rDistance,
            rRecord,
            rRecordPlaced,
            rRecordHolder[24],
            Float:rSpawnPositionX,
            Float:rSpawnPositionY,
            Float:rSpawnPositionA,
            Float:rSpawnPositionZ,

            Float:rCheckpointPositionX,
            Float:rCheckpointPositionY,
            Float:rCheckpointPositionZ,

            checkpointCount = 0,

            bool:firstline = false;

        while (fread(file, line))
        {
            if (!firstline)
            {
                firstline = true;

                if (!sscanf(line, "p<|>s[24]iiiiiis[24]ffff", rName, rVehicleID, rMaxCheckpoints, rInterior, rDistance, rRecord, rRecordPlaced, rRecordHolder, rSpawnPositionX, rSpawnPositionY, rSpawnPositionZ, rSpawnPositionA))
                {
                    format(RACE_rInfo[race][0][raceName], 24, "%s", rName);
                    RACE_rInfo[race][0][raceVehicleID] = rVehicleID;
                    RACE_rInfo[race][0][raceMaxCheckpoints] = rMaxCheckpoints;
                    RACE_rInfo[race][0][raceInterior] = rInterior;
                    RACE_rInfo[race][0][raceDistance] = rDistance;
                    RACE_rInfo[race][0][raceRecord] = rRecord;
                    RACE_rInfo[race][0][raceRecordPlaced] = rRecordPlaced;
                    format(RACE_rInfo[race][0][raceRecordHolder], 24, "%s", rRecordHolder);
                    RACE_rInfo[race][0][raceSpawnPositionX] = rSpawnPositionX;
                    RACE_rInfo[race][0][raceSpawnPositionY] = rSpawnPositionY;
                    RACE_rInfo[race][0][raceSpawnPositionZ] = rSpawnPositionZ;
                    RACE_rInfo[race][0][raceSpawnPositionA] = rSpawnPositionA;
                }
            }
            else
            {
                if (!sscanf(line, "p<|>fff", rCheckpointPositionX, rCheckpointPositionY, rCheckpointPositionZ))
                {
                    RACE_rInfo[race][checkpointCount][raceCheckpointPositionX] = rCheckpointPositionX;
                    RACE_rInfo[race][checkpointCount][raceCheckpointPositionY] = rCheckpointPositionY;
                    RACE_rInfo[race][checkpointCount][raceCheckpointPositionZ] = rCheckpointPositionZ;

                    checkpointCount++;
                }
            }
        }
        RACE_loadedRaces++;

        fclose(file);
    }
    return 1;
}

RACE_updateRace(race)
{
    new fileName[15];

    format(fileName, sizeof(fileName), "Races/%d.txt", race);

    new File:file = fopen(fileName, io_append);

    if (!file)
    {
        printf("[CORRIDA] %s Falhou para abrir.", fileName);
        return 0;
    }
    else
    {
        new string[200];

        format(string, sizeof(string), "%s|%i|%i|%i|%i|%i|%i|%s|%f|%f|%f|%f\r\n",
               RACE_rInfo[race][0][raceName],
               RACE_rInfo[race][0][raceVehicleID],
               RACE_rInfo[race][0][raceMaxCheckpoints],
               RACE_rInfo[race][0][raceInterior],
               RACE_rInfo[race][0][raceDistance],
               RACE_rInfo[race][0][raceRecord],
               RACE_rInfo[race][0][raceRecordPlaced],
               RACE_rInfo[race][0][raceRecordHolder],
               RACE_rInfo[race][0][raceSpawnPositionX],
               RACE_rInfo[race][0][raceSpawnPositionY],
               RACE_rInfo[race][0][raceSpawnPositionZ],
               RACE_rInfo[race][0][raceSpawnPositionA]);

        RACE_replaceline(fileName, RACE_rInfo[race][0][raceName], string);

        fclose(file);
    }
    return 1;
}

forward RACE_showPlayerTip(playerid, text[], time);
public RACE_showPlayerTip(playerid, text[], time)
{
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip2]);

    TextDrawSetString(RACE_pInfo[playerid][RACE_textdrawTip], text);
    TextDrawSetString(RACE_pInfo[playerid][RACE_textdrawTip2], text);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip]);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip2]);

    SetTimerEx("RACE_hidePlayerTip", time, false, "d", playerid);
    return 1;
}

forward RACE_hidePlayerTip(playerid, text[]);
public RACE_hidePlayerTip(playerid, text[])
{
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawTip2]);
    return 1;
}

forward RACE_hidePlayerSemaforoTextdraws(playerid);
public RACE_hidePlayerSemaforoTextdraws(playerid)
{
    // SEMAFORO TD's

    TextDrawColour(RACE_pInfo[playerid][RACE_textdrawCircleRed], -16777171);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleRed]);
    TextDrawColour(RACE_pInfo[playerid][RACE_textdrawCircleYellow], -65491);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleYellow]);
    TextDrawColour(RACE_pInfo[playerid][RACE_textdrawCircleGreen], 16711725);
    TextDrawShowForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleGreen]);
    for (new i = 0; i < 24; i++) TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawUsebox][i]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleRed]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleYellow]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawCircleGreen]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawSecondsLeft]);
    TextDrawHideForPlayer(playerid, RACE_pInfo[playerid][RACE_textdrawStartIn]);
}

forward RACE_createVehicle(playerid, vehicleid, Float:X, Float:Y, Float:Z, Float:A, pInterior, pVirtualWorld);
public RACE_createVehicle(playerid, vehicleid, Float:X, Float:Y, Float:Z, Float:A, pInterior, pVirtualWorld)
{
    RemovePlayerFromVehicle(playerid);

    if (RACE_pInfo[playerid][RACE_playerVehicle] != -1)
    {
        DestroyVehicle(RACE_pInfo[playerid][RACE_playerVehicle]);
    }

    new Veh = CreateVehicle(vehicleid, X, Y, Z + 2.0, A, random(200), random(200), -1);

    SetPlayerInterior(playerid, pInterior);
    SetPlayerVirtualWorld(playerid, pVirtualWorld);

    LinkVehicleToInterior(Veh, pInterior);
    SetVehicleVirtualWorld(Veh, pVirtualWorld);

    PutPlayerInVehicle(playerid, Veh, 0);

    SetPlayerArmedWeapon(playerid, WEAPON_FIST);

    RACE_pInfo[playerid][RACE_playerVehicle] = GetPlayerVehicleID(playerid);
    return 1;
}

forward RACE_prepareEvent(RaceID);
public RACE_prepareEvent(RaceID)
{
    KillTimer(RACE_countdownTimer);
    KillTimer(RACE_checkRaceTimer);

    new randomNumber = random(RACE_loadedRaces);

    if (RaceID == -1)
    {
        RACE_runningID = randomNumber;
    }
    else
    {
        RACE_runningID = RaceID;
    }

    RACE_isRaceStarted = false;
    RACE_isRaceInLobby = true;

    RACE_timeLeft = 300;
    RACE_timerCounter = 30;

    RACE_playersInEvent = 0;
    RACE_playersSpawned = 0;
    RACE_playersLeft = 0;

    for (new i = 0; i < MAX_CHECKS; i++)
    {
        RACE_checkpoints[i] = 0;
    }

    for (new i = 0; i < sizeof(RACE_spawnSlots); i++)
    {
        RACE_spawnSlots[i] = -1;
    }

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (RACE_pInfo[i][RACE_playerVehicle] != -1)
        {
            DestroyVehicle(RACE_pInfo[i][RACE_playerVehicle]);
            RACE_pInfo[i][RACE_playerVehicle] = -1;
        }
    }

    RACE_countDown();

    RACE_globalString[0] = '\0';
    format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ esta prestes a iniciar em 30 segundos. Use ~b~~h~~h~/race~w~ para participar!", RACE_rInfo[RACE_runningID][0][raceName]);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        notification.Show(i, "CORRIDA", RACE_globalString, "hud:radar_Flag");
    }

    GameTextForAll("~w~Uma corrida ~b~~h~~h~iniciou!", 2000, 3);

    format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} O recordista foi {1E90FF}%s{FFFFFF} com {1E90FF}%i:%02i.%03i{FFFFFF} feitos em {1E90FF}%s{FFFFFF}.", RACE_rInfo[RACE_runningID][0][raceRecordHolder], RACE_rInfo[RACE_runningID][0][raceRecord] / 60000, ((RACE_rInfo[RACE_runningID][0][raceRecord] / 1000) % 60), RACE_rInfo[RACE_runningID][0][raceRecord] % 1000, RACE_convertTime(RACE_rInfo[RACE_runningID][0][raceRecordPlaced], false));
    SendClientMessageToAll(0x1E90FF00, RACE_globalString);
    return 1;
}

forward RACE_checkRace();
public RACE_checkRace()
{
    if (RACE_playersLeft == 0)
    {
        RACE_finishEvent(2);
    }

    if (RACE_timeLeft == 0)
    {
        RACE_finishEvent(3);
    }
    else
    {
        RACE_timeLeft--;
        RACE_globalString[0] = '\0';
        format(RACE_globalString, sizeof(RACE_globalString), "%d:%02d", (RACE_timeLeft / 60) % 60, RACE_timeLeft % 60);

        foreach (new i : Player)
        {
            if (RACE_pInfo[i][RACE_isPlayerInRace])
            {
                TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][18], RACE_globalString);
                SetVehicleHealth(RACE_pInfo[i][RACE_playerVehicle], 1000);
            }
        }
    }
    return 1;
}

forward RACE_finishEvent(reason);
public RACE_finishEvent(reason)
{
    KillTimer(RACE_countdownTimer);
    KillTimer(RACE_checkRaceTimer);

    RACE_isRaceStarted = false;
    RACE_isRaceInLobby = false;

    foreach (new i : Player)
    {
        if (RACE_pInfo[i][RACE_isPlayerInRace])
        {
            TogglePlayerControllable(i, true);

            DisableRemoteVehicleCollisions(i, false);

            DisablePlayerCheckpoint(i);
            DisablePlayerRaceCheckpoint(i);

            RACE_hidePlayerRaceHudTextdraws(i);

            RemovePlayerFromVehicle(i);

            DestroyVehicle(RACE_pInfo[i][RACE_playerVehicle]);
            RACE_pInfo[i][RACE_playerVehicle] = -1;

            // SpawnPlayer(i);

            RACE_pInfo[i][RACE_isPlayerInRace] = false;
        }
    }

    RACE_globalString[0] = '\0';

    switch (reason)
    {
        case 0: format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ terminou.", RACE_rInfo[RACE_runningID][0][raceName]);
        case 1: format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ terminou. (Encerrado pelo administrador)", RACE_rInfo[RACE_runningID][0][raceName]);
        case 2: format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ terminou. (Todos os jogadores sairam)", RACE_rInfo[RACE_runningID][0][raceName]);
        case 3: format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ terminou. (Tempo esgotado)", RACE_rInfo[RACE_runningID][0][raceName]);
    }
    return 1;
}

forward RACE_countDown();
public RACE_countDown()
{
    KillTimer(RACE_countdownTimer);

    switch (RACE_timerCounter)
    {
        case 0:
        {
            if (RACE_playersInEvent == 0)
            {
                KillTimer(RACE_checkRaceTimer);
                KillTimer(RACE_countdownTimer);

                RACE_isRaceStarted = false;
                RACE_isRaceInLobby = false;

                RACE_timerCounter = 15;

                RACE_playersSpawned = 0;
                RACE_playersInEvent = 0;
                RACE_playersLeft = 0;

                RACE_globalString[0] = '\0';
                format(RACE_globalString, sizeof(RACE_globalString), "A corrida ~b~~h~~h~%s~w~ terminou. (Por falta de participantes)", RACE_rInfo[RACE_runningID][0][raceName]);
                for (new i = 0; i < MAX_PLAYERS; i++)
                {
                    notification.Show(i, "CORRIDA", RACE_globalString, "hud:radar_Flag");
                }
                return 1;
            }
            else
            {
                foreach (new i : Player)
                {
                    if (RACE_pInfo[i][RACE_isPlayerInRace])
                    {
                        LimparChat(i);
                        new runID = RACE_runningID;

                        RACE_showPlayerRaceHudTextdraws(i);

                        RACE_globalString[0] = '\0';
                        format(RACE_globalString, sizeof(RACE_globalString), "%s", RACE_rInfo[RACE_runningID][0][raceName]);
                        TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][20], RACE_globalString);

                        switch (RACE_pInfo[i][RACE_playerPosition])
                        {
                            case 1: format(RACE_globalString, sizeof(RACE_globalString), "%dst", RACE_pInfo[i][RACE_playerPosition]);
                            case 2: format(RACE_globalString, sizeof(RACE_globalString), "%dnd", RACE_pInfo[i][RACE_playerPosition]);
                            case 3: format(RACE_globalString, sizeof(RACE_globalString), "%drd", RACE_pInfo[i][RACE_playerPosition]);
                            default:
                                format(RACE_globalString, sizeof(RACE_globalString), "%dth", RACE_pInfo[i][RACE_playerPosition]);
                        }

                        TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][17], RACE_globalString);

                        for (new p = 0; p < MAX_PLAYERS; p++)
                        {
                            if (RACE_pInfo[p][RACE_playerPosition] > 0 && RACE_pInfo[p][RACE_playerPosition] <= MAX_PLAYERS)
                            {
                                new position = RACE_pInfo[p][RACE_playerPosition];
                                new textdrawIndex;

                                switch (position)
                                {
                                    case 1: textdrawIndex = 8;
                                    case 2: textdrawIndex = 12;
                                    case 3: textdrawIndex = 16;
                                    default:
                                        textdrawIndex = 4;
                                }

                                format(RACE_globalString, sizeof(RACE_globalString), "%s", RACE_pInfo[p][RACE_playerUsername]);
                                TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][textdrawIndex], RACE_globalString);
                            }
                        }

                        format(RACE_globalString, sizeof(RACE_globalString), "%d/%d", RACE_pInfo[i][RACE_playerCheckpointsPassed], RACE_rInfo[RACE_runningID][0][raceMaxCheckpoints]);
                        TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][0], RACE_globalString);

                        SetPlayerRaceCheckpoint(i, CP_TYPE_GROUND_NORMAL, RACE_rInfo[runID][0][raceCheckpointPositionX], RACE_rInfo[runID][0][raceCheckpointPositionY], RACE_rInfo[runID][0][raceCheckpointPositionZ], RACE_rInfo[runID][1][raceCheckpointPositionX], RACE_rInfo[runID][1][raceCheckpointPositionY], RACE_rInfo[runID][1][raceCheckpointPositionZ], 7.0);

                        DisablePlayerCheckpoint(i);
                        SetPlayerCheckpoint(i, RACE_rInfo[runID][1][raceCheckpointPositionX], RACE_rInfo[runID][1][raceCheckpointPositionY], RACE_rInfo[runID][1][raceCheckpointPositionZ], 0.0);

                        GameTextForPlayer(i, "~n~~g~Go!", 2000, 3);
                        PlayerPlaySound(i, 3200, 0.0, 0.0, 0.0);

                        TogglePlayerControllable(i, true);

                        DisableRemoteVehicleCollisions(i, true);

                        RACE_pInfo[i][RACE_playerTimeTookToFinish] = GetTickCount();

                        format(RACE_globalString, sizeof(RACE_globalString), "A corrida iniciou com ~g~%d~w~ jogadores.~n~Use ~r~/exitrace~w~ se quiser sair.~n~Boa sorte!", RACE_playersSpawned);
                        RACE_showPlayerTip(i, RACE_globalString, 10000);

                        SetCameraBehindPlayer(i);
                    }
                }

                RACE_globalString[0] = '\0';
                format(RACE_globalString, sizeof(RACE_globalString), "A corrida de ~b~~h~~h~%s~w~ iniciou!", RACE_rInfo[RACE_runningID][0][raceName]);
                for (new i = 0; i < MAX_PLAYERS; i++)
                {
                    notification.Show(i, "CORRIDA", RACE_globalString, "hud:radar_Flag");
                }

                RACE_checkRaceTimer = SetTimer("RACE_checkRace", 1000, true);

                RACE_playersLeft = RACE_playersInEvent;

                RACE_isRaceStarted = true;
                RACE_isRaceInLobby = false;

                return 1;
            }
        }
        case 1:
        {
            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "~g~%ds", RACE_timerCounter);

            foreach (new i : Player)
            {
                if (RACE_pInfo[i][RACE_isPlayerInRace])
                {
                    PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
                    TextDrawSetString(RACE_pInfo[i][RACE_textdrawSecondsLeft], RACE_globalString);
                    TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleGreen], 0x00FF00FF);
                    TextDrawShowForPlayer(i, RACE_pInfo[i][RACE_textdrawCircleGreen]);
                    TogglePlayerControllable(i, false);
                    SetTimerEx("RACE_hidePlayerSemaforoTextdraws", 800, false, "d", i);
                }
            }
        }
        case 2:
        {
            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "~g~%ds", RACE_timerCounter);

            foreach (new i : Player)
            {
                if (RACE_pInfo[i][RACE_isPlayerInRace])
                {
                    PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
                    TextDrawSetString(RACE_pInfo[i][RACE_textdrawSecondsLeft], RACE_globalString);
                    TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleYellow], 0xFFFF00FF);
                    TextDrawShowForPlayer(i, RACE_pInfo[i][RACE_textdrawCircleYellow]);
                }
            }
        }
        case 3:
        {
            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "~g~%ds", RACE_timerCounter);

            foreach (new i : Player)
            {
                if (RACE_pInfo[i][RACE_isPlayerInRace])
                {
                    PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
                    TextDrawSetString(RACE_pInfo[i][RACE_textdrawSecondsLeft], RACE_globalString);
                    TextDrawColour(RACE_pInfo[i][RACE_textdrawCircleRed], 0xFF0000FF);
                    TextDrawShowForPlayer(i, RACE_pInfo[i][RACE_textdrawCircleRed]);
                    SetCameraBehindPlayer(i);
                    TogglePlayerControllable(i, false);
                }
            }
        }
        default:
        {
            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "~r~%ds", RACE_timerCounter);

            foreach (new i : Player)
            {
                if (RACE_pInfo[i][RACE_isPlayerInRace])
                {
                    RACE_showPlayerSemaforoTextdraws(i);
                    TextDrawSetString(RACE_pInfo[i][RACE_textdrawSecondsLeft], RACE_globalString);
                    TogglePlayerControllable(i, false);
                }
            }

            if (RACE_timerCounter == 15)
            {
                format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} A corrida de %s esta prestes a começar em 15 segundos. Use {1E90FF}/race{FFFFFF} para participar!", RACE_rInfo[RACE_runningID][0][raceName]);
                SendClientMessageToAll(0x1E90FF00, RACE_globalString);
            }

            if (RACE_timerCounter == 5)
            {
                for (new i = 0; i < MAX_PLAYERS; i++)
                {
                    LimparChat(i);
                }
                format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} A corrida de %s esta prestes a começar em 5 segundos. Use {1E90FF}/race{FFFFFF} para participar!", RACE_rInfo[RACE_runningID][0][raceName]);
                SendClientMessageToAll(0x1E90FF00, RACE_globalString);
            }
        }
    }

    RACE_countdownTimer = SetTimerEx("RACE_countDown", 1000, false, "d", --RACE_timerCounter);
    return 1;
}

forward RACE_playerFinishedTheRace(playerid);
public RACE_playerFinishedTheRace(playerid)
{
    if (!RACE_pInfo[playerid][RACE_isPlayerInRace]) return 0;

    switch (RACE_pInfo[playerid][RACE_playerPosition])
    {
        case 1:
        {
            RACE_pInfo[playerid][RACE_playerTimeTookToFinish] = GetTickCount() - RACE_pInfo[playerid][RACE_playerTimeTookToFinish];

            if (RACE_rInfo[RACE_runningID][0][raceRecord] == 0)
            {
                strcat((RACE_rInfo[RACE_runningID][0][raceRecordHolder][0] = EOS, RACE_rInfo[RACE_runningID][0][raceRecordHolder]), RACE_pInfo[playerid][RACE_playerUsername], MAX_PLAYER_NAME);

                RACE_rInfo[RACE_runningID][0][raceRecord] = RACE_pInfo[playerid][RACE_playerTimeTookToFinish];
                RACE_rInfo[RACE_runningID][0][raceRecordPlaced] = gettime();

                RACE_updateRace(RACE_runningID);

                RACE_globalString[0] = '\0';
                format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} %s terminou em primeiro e quebrou o recorde atual para {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerUsername], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
                SendClientMessageToAll(0x00ff8000, RACE_globalString);

                /* NOTICE: Give Awards */
            }
            else if (RACE_rInfo[RACE_runningID][0][raceRecord] > RACE_pInfo[playerid][RACE_playerTimeTookToFinish])
            {
                strcat((RACE_rInfo[RACE_runningID][0][raceRecordHolder][0] = EOS, RACE_rInfo[RACE_runningID][0][raceRecordHolder]), RACE_pInfo[playerid][RACE_playerUsername], MAX_PLAYER_NAME);

                RACE_rInfo[RACE_runningID][0][raceRecord] = RACE_pInfo[playerid][RACE_playerTimeTookToFinish];
                RACE_rInfo[RACE_runningID][0][raceRecordPlaced] = gettime();

                RACE_updateRace(RACE_runningID);

                RACE_globalString[0] = '\0';
                format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} %s terminou em primeiro e quebrou o recorde atual para {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerUsername], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
                SendClientMessageToAll(0x00ff8000, RACE_globalString);

                /* NOTICE: Give Awards */
            }
            else
            {
                RACE_globalString[0] = '\0';
                format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} %s terminou em 1º lugar em {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerUsername], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
                SendClientMessageToAll(0x00ff8000, RACE_globalString);

                /* NOTICE: Give Awards */
            }
        }
        case 2:
        {
            RACE_pInfo[playerid][RACE_playerTimeTookToFinish] = GetTickCount() - RACE_pInfo[playerid][RACE_playerTimeTookToFinish];

            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} %s terminou em 2º lugar em {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerUsername], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
            SendClientMessageToAll(0x00ff8000, RACE_globalString);

            /* NOTICE: Give Awards */
        }
        case 3:
        {
            RACE_pInfo[playerid][RACE_playerTimeTookToFinish] = GetTickCount() - RACE_pInfo[playerid][RACE_playerTimeTookToFinish];

            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} %s terminou em 3º lugar em {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerUsername], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
            SendClientMessageToAll(0x00ff8000, RACE_globalString);

            /* NOTICE: Give Awards */
        }
        default:
        {
            RACE_pInfo[playerid][RACE_playerTimeTookToFinish] = GetTickCount() - RACE_pInfo[playerid][RACE_playerTimeTookToFinish];

            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "[CORRIDA]{FFFFFF} Voce terminou em %dº lugar {00ff80}%i:%02i.%03i{FFFFFF}.", RACE_pInfo[playerid][RACE_playerPosition], RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 60000, ((RACE_pInfo[playerid][RACE_playerTimeTookToFinish] / 1000) % 60), RACE_pInfo[playerid][RACE_playerTimeTookToFinish] % 1000);
            SendClientMessage(playerid, 0x00ff8000, RACE_globalString);
        }
    }

    TogglePlayerControllable(playerid, true);
    DisableRemoteVehicleCollisions(playerid, false);

    RACE_hidePlayerRaceHudTextdraws(playerid);

    RemovePlayerFromVehicle(playerid);

    DestroyVehicle(RACE_pInfo[playerid][RACE_playerVehicle]);
    RACE_pInfo[playerid][RACE_playerVehicle] = -1;

    // SpawnPlayer(playerid);

    RACE_pInfo[playerid][RACE_isPlayerInRace] = false;
    return 1;
}

forward RACE_playerJoinEvent(playerid);
public RACE_playerJoinEvent(playerid)
{
    new distance = RACE_rInfo[RACE_runningID][0][raceDistance],
        rID = RACE_runningID,
        Float:X,
        Float:Y,
        Float:Z = RACE_rInfo[rID][0][raceSpawnPositionZ],
        Float:angle = RACE_rInfo[rID][0][raceSpawnPositionA],
        slot;

    for (new i = 0, j = sizeof(RACE_spawnSlots); i < j; i++)
    {
        if (RACE_spawnSlots[i] == -1)
        {
            slot = i;
            break;
        }
        else
        {
            continue;
        }
    }

    RACE_spawnSlots[slot] = playerid;

    switch (slot)
    {
        case 0:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX], Y = RACE_rInfo[rID][0][raceSpawnPositionY];
        }
        case 1:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] + (distance * floatcos(angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] + (distance * floatsin(angle, degrees));
        }
        case 2:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 2) * floatcos(angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 2) * floatsin(angle, degrees));
        }
        case 3:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 3) * floatcos(angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 3) * floatsin(angle, degrees));
        }
        case 4:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] - (6 * floatsin(- angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] - (6 * floatcos(- angle, degrees));
        }
        case 5:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + (distance * floatcos(angle, degrees)) - (6 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + (distance * floatsin(angle, degrees)) - (6 * floatcos(- angle, degrees)));
        }
        case 6:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 2) * floatcos(angle, degrees)) - (6 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 2) * floatsin(angle, degrees)) - (6 * floatcos(- angle, degrees)));
        }
        case 7:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 3) * floatcos(angle, degrees)) - (6 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 3) * floatsin(angle, degrees)) - (6 * floatcos(- angle, degrees)));
        }
        case 8:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] - (12 * floatsin(- angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] - (12 * floatcos(- angle, degrees));
        }
        case 9:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + (distance  * floatcos(angle, degrees)) - (12 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + (distance  * floatsin(angle, degrees)) - (12 * floatcos(- angle, degrees)));
        }
        case 10:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 2) * floatcos(angle, degrees)) - (12 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 2) * floatsin(angle, degrees)) - (12 * floatcos(- angle, degrees)));
        }
        case 11:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 3) * floatcos(angle, degrees)) - (12 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 3) * floatsin(angle, degrees)) - (12 * floatcos(- angle, degrees)));
        }
        case 12:
        {
            X = RACE_rInfo[rID][0][raceSpawnPositionX] - (18 * floatsin(- angle, degrees));
            Y = RACE_rInfo[rID][0][raceSpawnPositionY] - (18 * floatcos(- angle, degrees));
        }
        case 13:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + (distance * floatcos(angle, degrees)) - (18 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + (distance * floatsin(angle, degrees)) - (18 * floatcos(- angle, degrees)));
        }
        case 14:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 2) * floatcos(angle, degrees)) - (18 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 2) * floatsin(angle, degrees)) - (18 * floatcos(- angle, degrees)));
        }
        case 15:
        {
            X = (RACE_rInfo[rID][0][raceSpawnPositionX] + ((distance * 3) * floatcos(angle, degrees)) - (18 * floatsin(- angle, degrees)));
            Y = (RACE_rInfo[rID][0][raceSpawnPositionY] + ((distance * 3) * floatsin(angle, degrees)) - (18 * floatcos(- angle, degrees)));
        }
    }

    SetPlayerInterior(playerid, RACE_rInfo[RACE_runningID][0][raceInterior]);
    SetPlayerVirtualWorld(playerid, RACE_VIRTUALWORLD);

    RACE_createVehicle(playerid, RACE_rInfo[RACE_runningID][0][raceVehicleID], X, Y, Z, angle, RACE_rInfo[RACE_runningID][0][raceInterior], RACE_VIRTUALWORLD);

    X = RACE_rInfo[RACE_runningID][0][raceSpawnPositionX] + (distance * 1.5) * floatcos(angle, degrees);
    Y = RACE_rInfo[RACE_runningID][0][raceSpawnPositionY] + (distance * 1.5) * floatsin(angle, degrees);
    SetPlayerCameraLookAt(playerid, X, Y, Z);

    X = (RACE_rInfo[RACE_runningID][0][raceSpawnPositionX] + ((distance * 1.5) * floatcos(angle, degrees)) + (12 * floatsin(- angle, degrees)));
    Y = (RACE_rInfo[RACE_runningID][0][raceSpawnPositionY] + ((distance * 1.5) * floatsin(angle, degrees)) + (12 * floatcos(- angle, degrees)));
    SetPlayerCameraPos(playerid, X, Y, Z + 6);

    TogglePlayerControllable(playerid, false);

    RACE_playersInEvent++;
    RACE_playersSpawned++;

    RACE_pInfo[playerid][RACE_playerCheckpointsPassed] = 0;
    RACE_pInfo[playerid][RACE_playerPosition] = slot + 1;

    RACE_pInfo[playerid][RACE_isPlayerInRace] = true;
    return 1;
}

forward RACE_playerExitEvent(playerid);
public RACE_playerExitEvent(playerid)
{
    TogglePlayerControllable(playerid, true);
    DisableRemoteVehicleCollisions(playerid, false);

    DisablePlayerCheckpoint(playerid);
    DisablePlayerRaceCheckpoint(playerid);

    RACE_playersInEvent--;
    RACE_playersLeft--;

    if (RACE_isRaceInLobby)
    {
        for (new i = 0; i < sizeof(RACE_spawnSlots); i++)
        {
            if (RACE_spawnSlots[i] == playerid)
            {
                RACE_spawnSlots[i] = -1;
                break;
            }
        }
    }

    RACE_hidePlayerRaceTextdraws(playerid);
    RACE_hidePlayerRaceHudTextdraws(playerid);

    RemovePlayerFromVehicle(playerid);

    DestroyVehicle(RACE_pInfo[playerid][RACE_playerVehicle]);
    RACE_pInfo[playerid][RACE_playerVehicle] = -1;

    SpawnPlayer(playerid);

    RACE_pInfo[playerid][RACE_isPlayerInRace] = false;
    return 1;
}


public OnGameModeInit()
{
    for (new i = 0; i < MAX_RACES; i++)
    {
        RACE_loadRaceFromFile(i);
    }

    printf(" ___________________________________________________________\n");
    printf("  » Raf Racing System loaded. (Loaded %d Races)", RACE_loadedRaces);
    printf(" ___________________________________________________________\n\n\n");

    RACE_loadTextdraws();

	#if defined RACE_OnGameModeInit
    RACE_OnGameModeInit();
    #endif
    return 1;
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit RACE_OnGameModeInit

#if defined RACE_OnGameModeInit
forward RACE_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
    GetPlayerName(playerid, RACE_pInfo[playerid][RACE_playerUsername], MAX_PLAYER_NAME);

    RACE_resetPlayerVariables(playerid);

	#if defined RACE_OnPlayerConnect
    RACE_OnPlayerConnect(playerid);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect RACE_OnPlayerConnect

#if defined RACE_OnPlayerConnect
forward RACE_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if (RACE_pInfo[playerid][RACE_isPlayerInRace])
    {
        RACE_playerExitEvent(playerid);
    }

	#if defined RACE_OnPlayerDisconnect
    RACE_OnPlayerDisconnect(playerid, reason);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect RACE_OnPlayerDisconnect

#if defined RACE_OnPlayerDisconnect
forward RACE_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if (RACE_pInfo[playerid][RACE_isPlayerInRace])
    {
        RACE_playerExitEvent(playerid);
    }

	#if defined RACE_OnPlayerDeath
    RACE_OnPlayerDeath(playerid, killerid, reason);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath RACE_OnPlayerDeath

#if defined RACE_OnPlayerDeath
forward RACE_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (RACE_pInfo[playerid][RACE_isPlayerInRace])
    {
        if (oldstate == PLAYER_STATE_DRIVER)
        {
            PutPlayerInVehicle(playerid, RACE_pInfo[playerid][RACE_playerVehicle], 0);
        }
    }

	#if defined RACE_OnPlayerStateChange
    RACE_OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerStateChange
    #undef OnPlayerStateChange
#else
    #define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange RACE_OnPlayerStateChange

#if defined RACE_OnPlayerStateChange
forward RACE_OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate);
#endif

public OnPlayerUpdate(playerid)
{
    if (RACE_isRaceStarted)
    {
        if (RACE_pInfo[playerid][RACE_isPlayerInRace])
        {
            new speed = RACE_getPlayerSpeed(playerid), string[32];
            format(string, sizeof(string), "~g~Velocidade:~w~ %d KM/H", speed);
            TextDrawSetString(RACE_pInfo[playerid][RACE_textdrawSpeed], string);
        }
    }

	#if defined RACE_OnPlayerUpdate
    RACE_OnPlayerUpdate(playerid);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate RACE_OnPlayerUpdate

#if defined RACE_OnPlayerUpdate
forward RACE_OnPlayerUpdate(playerid);
#endif

public OnPlayerEnterRaceCheckpoint(playerid)
{
    if (RACE_pInfo[playerid][RACE_isPlayerInRace])
    {
        if (IsPlayerInAnyVehicle(playerid))
        {
            RACE_pInfo[playerid][RACE_playerCheckpointsPassed]++;
            RACE_checkpoints[RACE_pInfo[playerid][RACE_playerCheckpointsPassed]]++;
            RACE_pInfo[playerid][RACE_playerPosition] = RACE_checkpoints[RACE_pInfo[playerid][RACE_playerCheckpointsPassed]];

            if (RACE_pInfo[playerid][RACE_playerCheckpointsPassed] == RACE_rInfo[RACE_runningID][0][raceMaxCheckpoints])
            {
                RACE_playersLeft--;

                DisablePlayerRaceCheckpoint(playerid);
                DisablePlayerCheckpoint(playerid);

                RACE_playerFinishedTheRace(playerid);

                PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
            }
            else
            {
                new i = RACE_pInfo[playerid][RACE_playerCheckpointsPassed],
                    runID = RACE_runningID;

                DisablePlayerCheckpoint(playerid);
                DisablePlayerRaceCheckpoint(playerid);

                if (RACE_rInfo[runID][0][raceMaxCheckpoints] - 1 == i)
                {
                    SetPlayerRaceCheckpoint(playerid, CP_TYPE_GROUND_FINISH, RACE_rInfo[runID][i][raceCheckpointPositionX], RACE_rInfo[runID][i][raceCheckpointPositionY], RACE_rInfo[runID][i][raceCheckpointPositionZ], RACE_rInfo[runID][i + 1][raceCheckpointPositionX], RACE_rInfo[runID][i + 1][raceCheckpointPositionY], RACE_rInfo[runID][i + 1][raceCheckpointPositionZ], 7.0);
                }
                else
                {
                    SetPlayerRaceCheckpoint(playerid, CP_TYPE_GROUND_NORMAL, RACE_rInfo[runID][i][raceCheckpointPositionX], RACE_rInfo[runID][i][raceCheckpointPositionY], RACE_rInfo[runID][i][raceCheckpointPositionZ], RACE_rInfo[runID][i + 1][raceCheckpointPositionX], RACE_rInfo[runID][i + 1][raceCheckpointPositionY], RACE_rInfo[runID][i + 1][raceCheckpointPositionZ], 7.0);

                    SetPlayerCheckpoint(playerid, RACE_rInfo[runID][i + 1][raceCheckpointPositionX], RACE_rInfo[runID][i + 1][raceCheckpointPositionY], RACE_rInfo[runID][i + 1][raceCheckpointPositionZ], 0.0);
                }

                RACE_globalString[0] = '\0';

                switch (RACE_pInfo[i][RACE_playerPosition])
                {
                    case 1: format(RACE_globalString, sizeof(RACE_globalString), "%dst", RACE_pInfo[i][RACE_playerPosition]);
                    case 2: format(RACE_globalString, sizeof(RACE_globalString), "%dnd", RACE_pInfo[i][RACE_playerPosition]);
                    case 3: format(RACE_globalString, sizeof(RACE_globalString), "%drd", RACE_pInfo[i][RACE_playerPosition]);
                    default:
                        format(RACE_globalString, sizeof(RACE_globalString), "%dth", RACE_pInfo[i][RACE_playerPosition]);
                }

                TextDrawSetString(RACE_pInfo[i][RACE_textdrawRaceHud][17], RACE_globalString);

                for (new p = 0; p < MAX_PLAYERS; p++)
                {
                    if (RACE_pInfo[p][RACE_playerPosition] > 0 && RACE_pInfo[p][RACE_playerPosition] <= MAX_PLAYERS)
                    {
                        new position = RACE_pInfo[p][RACE_playerPosition];
                        new textdrawIndex;

                        switch (position)
                        {
                            case 1: textdrawIndex = 8;
                            case 2: textdrawIndex = 12;
                            case 3: textdrawIndex = 16;
                            default:
                                textdrawIndex = 4;
                        }

                        format(RACE_globalString, sizeof(RACE_globalString), "%s", RACE_pInfo[p][RACE_playerUsername]);
                        TextDrawSetString(RACE_pInfo[playerid][RACE_textdrawRaceHud][textdrawIndex], RACE_globalString);
                    }
                }

                format(RACE_globalString, sizeof(RACE_globalString), "%d/%d", i, RACE_rInfo[runID][0][raceMaxCheckpoints]);
                TextDrawSetString(RACE_pInfo[playerid][RACE_textdrawRaceHud][0], RACE_globalString);
                PlayerPlaySound(playerid, 1138, 0.0, 0.0, 0.0);
            }

            if (RACE_playersLeft == 0)
            {
                RACE_finishEvent(0);
            }
        }
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_RACE)
    {
        if (response)
        {
            RACE_prepareEvent(listitem);
        }
    }

	#if defined RACE_OnDialogResponse
    RACE_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
    #endif
    return 1;
}

#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse RACE_OnDialogResponse

#if defined RACE_OnDialogResponse
forward RACE_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if (RACE_pInfo[playerid][RACE_isPlayerInRace])
    {
        return 0;
    }

	#if defined RACE_OnPlayerExitVehicle
    RACE_OnPlayerExitVehicle(playerid, vehicleid);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerExitVehicle
    #undef OnPlayerExitVehicle
#else
    #define _ALS_OnPlayerExitVehicle
#endif

#define OnPlayerExitVehicle RACE_OnPlayerExitVehicle

#if defined RACE_OnPlayerExitVehicle
forward RACE_OnPlayerExitVehicle(playerid, vehicleid);
#endif

CMD:race(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    if (RACE_isRaceStarted) return notification.Show(playerid, "Erro!", "A corrida ja iniciou.", "ld_chat:badchat");
    if (!RACE_isRaceInLobby) return notification.Show(playerid, "Erro!", "Nao ha corrida em andamento no momento.", "ld_chat:badchat");
    if (RACE_playersSpawned > 15) notification.Show(playerid, "Erro!", "A corrida nao pode adicionar mais jogadores. Experimente a proxima.", "ld_chat:badchat");

    RACE_globalString[0] = '\0';
    format(RACE_globalString, sizeof(RACE_globalString), "~b~~h~~h~%s(%d)~w~ entrou na corrida. (%d/16) ~b~~h~~h~(/race)", RACE_pInfo[playerid][RACE_playerUsername], playerid, RACE_playersInEvent + 1);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        notification.Show(i, "CORRIDA", RACE_globalString, "hud:radar_Flag");
    }

    RACE_pInfo[playerid][RACE_isPlayerInRace] = true;

    RACE_playerJoinEvent(playerid);
    return 1;
}

CMD:exitrace(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    if (!RACE_pInfo[playerid][RACE_isPlayerInRace]) return notification.Show(playerid, "Erro!", "~b~~h~~h~»~w~ Voce nao esta em nenhuma corrida.", "ld_chat:badchat");

    RACE_playerExitEvent(playerid);

    RACE_globalString[0] = '\0';
    format(RACE_globalString, sizeof(RACE_globalString), "Voce saiu da corrida ~b~~h~~h~%s~w~.", RACE_rInfo[RACE_runningID][0][raceName]);
    notification.Show(playerid, "Corrida", RACE_globalString, "hud:radar_Flag");
    return 1;
}

CMD:startrace(playerid, params[])
{
    if (!IsPlayerConnected(playerid)) return 0;
    if (RACE_loadedRaces == 0) return notification.Show(playerid, "Erro!", "Nenhuma corrida encontrada.", "ld_chat:badchat");
    if (RACE_isRaceStarted) return notification.Show(playerid, "Erro!", "A corrida ja iniciou.", "ld_chat:badchat");
    if (RACE_isRaceInLobby) return notification.Show(playerid, "Erro!", "A corrida esta prestes a iniciar.", "ld_chat:badchat");

    new string[MAX_RACES * 16];

    for (new i = 0; i < RACE_loadedRaces; i++)
    {
        format(string, sizeof(string), "%s%s\n", string, RACE_rInfo[i][0][raceName]);
    }

    ShowPlayerDialog(playerid, DIALOG_RACE, DIALOG_STYLE_LIST, "Corrida » Selecione", string, "Selecionar", "Fechar");
    return 1;
}

CMD:endrace(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;
    if (!RACE_isRaceStarted) return notification.Show(playerid, "Erro!", "Nao ha corrida.", "ld_chat:badchat");
    RACE_finishEvent(1);
    return 1;
}

CMD:records(playerid, params[])
{
    new string[128];

    for (new i = 0; i < RACE_loadedRaces; i++)
    {
        format(string, sizeof(string), "%s{009900}%s:{FFFFFF} %s (%i:%02i.%03i)\n\n", string, RACE_rInfo[i][0][raceName], RACE_rInfo[i][0][raceRecordHolder], RACE_rInfo[i][0][raceRecord] / 60000, ((RACE_rInfo[i][0][raceRecord] / 1000) % 60), RACE_rInfo[i][0][raceRecord] % 1000);
    }

    ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Detentores de recordes de corrida", string, "Fechar", "");
    return 1;
}

CMD:resetrecord(playerid, params[])
{
    if (!IsPlayerConnected(playerid)) return 0;

    new rName[24];

    if (sscanf(params, "s[24]", rName)) return notification.Show(playerid, "Comando Admin", "Use: /resetrecord [Nome]", "ld_chat:badchat");

    for (new i = 0; i < RACE_loadedRaces; i++)
    {
        if (!strcmp(rName, RACE_rInfo[i][0][raceName], false))
        {
            strcat((RACE_rInfo[i][0][raceRecordHolder][0] = EOS, RACE_rInfo[i][0][raceRecordHolder]), "Nenhum", MAX_PLAYER_NAME);
            RACE_rInfo[i][0][raceRecord] = 0;
            RACE_rInfo[i][0][raceRecordPlaced] = 0;

            RACE_updateRace(i);

            RACE_globalString[0] = '\0';
            format(RACE_globalString, sizeof(RACE_globalString), "O recorde %s foi redefinido.", rName);
            notification.Show(playerid, "Comando Admin", RACE_globalString, "ld_chat:thumbup");

            return 1;
        }
    }

    notification.Show(playerid, "Erro!", "Esta corrida nao existe.", "ld_chat:badchat");
    return 1;
}

new idveh[MAX_PLAYERS] = 0;
new bool:criouveiculo[MAX_PLAYERS] = false;

CMD:cveh(playerid, params[])
{
    new id, cor1, cor2, Float:pos[3], Float:Angle;
    if (sscanf(params, "dD(-1)D(-1)", id, cor1, cor2)) return notification.Show(playerid, "Comando Admin", "Use: /cveh [id] [cor1] [cor2]", "ld_chat:badchat");
    if (IsPlayerInAnyVehicle(playerid)) notification.Show(playerid, "Erro!", "Saia do veiculo para criar outro.", "ld_chat:badchat");
    if (id < 400 || id > 611) return notification.Show(playerid, "Erro!", "Veiculo invalido. Tente IDs entre 400 e 611.", "ld_chat:badchat");

    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, Angle);

    idveh[playerid] = AddStaticVehicleEx(id, pos[0], pos[1], pos[2], Angle, cor1, cor2, -1);
    PutPlayerInVehicle(playerid, idveh[playerid], 0);
    LinkVehicleToInterior(idveh[playerid], GetPlayerInterior(playerid));
    SetVehicleVirtualWorld(idveh[playerid], GetPlayerVirtualWorld(playerid));
    criouveiculo[playerid] = true;
    return 1;
}

CMD:dveh(playerid, params[])
{

    new targetVehicleid;
    targetVehicleid = GetPlayerVehicleID(playerid);

    if (targetVehicleid != INVALID_VEHICLE_ID)
    {
        DestroyVehicle(targetVehicleid);
        StopAudioStreamForPlayer(playerid);
        return 1;
    }

    return 0;
}

new Float:ClickedMap[MAX_PLAYERS][3]; // TELEPORTAR CLICANDO NO MAPA - ADMIN
public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if (ClickedMap[playerid][0] == 0.0 && ClickedMap[playerid][1] == 0.0 && ClickedMap[playerid][2] == 0.0)
    {
        if (ClickedMap[playerid][0] != fX && ClickedMap[playerid][1] != fY && ClickedMap[playerid][2] != fZ)
        {
            ClickedMap[playerid][0] = fX;
            ClickedMap[playerid][1] = fY;
            ClickedMap[playerid][2] = fZ;
            if (!IsPlayerInAnyVehicle(playerid))
            {
                SetPlayerPosFindZ(playerid, fX, fY + 1, fZ + 1);
                ClickedMap[playerid][0] = 0.0;
                ClickedMap[playerid][1] = 0.0;
                ClickedMap[playerid][2] = 0.0;
            }
            else if (IsPlayerInAnyVehicle(playerid))
            {
                new Babatz = GetPlayerVehicleID(playerid);
                new Batz = GetPlayerVehicleSeat(playerid);
                SetVehiclePos(Babatz, fX, fY + 1, fZ + 1);
                PutPlayerInVehicle(playerid, Babatz, Batz);
                ClickedMap[playerid][0] = 0.0;
                ClickedMap[playerid][1] = 0.0;
                ClickedMap[playerid][2] = 0.0;
            }
        }
    }
    return 1;
}

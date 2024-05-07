#include <open.mp>
#include <race>
#include <YSI-Includes\YSI_Coding\y_hooks>

/*
     ___      _
    / __| ___| |_ _  _ _ __
    \__ \/ -_)  _| || | '_ \
    |___/\___|\__|\_,_| .__/
                      |_|
*/

// CHAT BUBBLE
#define MAX_CHAT_LENGTH         (144)
#define CHAT_DISTANCE           (20.0)

//---------------- SISTEMAS ----------------
#include "../systems/iradio.pwn"

main()
{
    printf(" ");
    printf("  -------------------------------");
    printf("  |  Cidade carregada com sucesso! |");
    printf("  -------------------------------");
    printf(" ");
}

public OnGameModeInit()
{
    SetWeather(0);
    UsePlayerPedAnims();
    DisableInteriorEnterExits();
    AddPlayerClass(0, 2495.3547, -1688.2319, 13.6774, 351.1646);
    AddStaticVehicle(522, 2493.7583, -1683.6482, 12.9099, 270.8069, -1, -1);
    SetGameModeText("Upper Horizon v0.1");
    SendRconCommand("language Portugues - Brasil");
    SendRconCommand("hostname Upper Horizon");
    SendRconCommand("mapname Brasil");
    return 1;
}

public OnGameModeExit()
{
    return 1;
}

/*
      ___
     / __|___ _ __  _ __  ___ _ _
    | (__/ _ \ '  \| '  \/ _ \ ' \
     \___\___/_|_|_|_|_|_\___/_||_|

*/

public OnPlayerConnect(playerid)
{
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SpawnPlayerPier(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid, 0);
    LimparChat(playerid);
    return 1;
}

/*
     ___              _      _ _    _
    / __|_ __  ___ __(_)__ _| (_)__| |_
    \__ \ '_ \/ -_) _| / _` | | (_-<  _|
    |___/ .__/\___\__|_\__,_|_|_/__/\__|
        |_|
*/

public OnPlayerText(playerid, text[])
{
    if (strlen(text) > 0 && strlen(text) <= MAX_CHAT_LENGTH)
    {
        SetPlayerChatBubble(playerid, text, 0x1E90FFFF, CHAT_DISTANCE, 5000);
    }
    return 0;
}

public OnRconLoginAttempt(ip[], password[], success)
{
    new authorizedIP[] = "45.188.223.203";
    if (!strfind(ip, authorizedIP))
    {
        printf("[LOG] Tentativa de login RCON falhou de %s. Acesso não autorizado.", ip);
        return 0;
    }

    if (success)
    {
        printf("[LOG] Tentativa de login RCON bem-sucedida de %s", ip);
    }
    else
    {
        printf("[LOG] Tentativa de login RCON falhou de %s com senha '%s'", ip, password);
    }
    return 1;
}

public OnScriptCash(playerid, amount, source)
{
    return 1;
}

stock SpawnPlayerPier(playerid)
{
    TogglePlayerSpectating(playerid, false);
    TogglePlayerControllable(playerid, true);
    CancelSelectTextDraw(playerid);
    SetSpawnInfo(playerid, 0, 154, 369.7811, -2050.6787, 7.8359, 359.9895, WEAPON_SAWEDOFF, 0, WEAPON_UZI, 0, WEAPON_FIST, 0);
    SetPlayerSkin(playerid, 154);
    SpawnPlayer(playerid);
    SetCameraBehindPlayer(playerid);
    LimparChat(playerid);
    return 1;
}

stock LimparChat(playerid)
{
    for (new i = 0; i < 100; i++) SendClientMessage(playerid, -1, "");
    return 1;
}

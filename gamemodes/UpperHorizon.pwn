#include <open.mp>
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
    SetGameModeText("Upper Horizon v0.1");
    AddPlayerClass(0, 2495.3547, -1688.2319, 13.6774, 351.1646);
    AddStaticVehicle(522, 2493.7583, -1683.6482, 12.9099, 270.8069, -1, -1);
    DisableInteriorEnterExits();
    SetWeather(0);
    SendRconCommand("language PT-BR");
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
    SetPlayerPos(playerid, 217.8511, -98.4865, 1005.2578);
    SetPlayerFacingAngle(playerid, 113.8861);
    SetPlayerInterior(playerid, 15);
    SetPlayerCameraPos(playerid, 215.2182, -99.5546, 1006.4);
    SetPlayerCameraLookAt(playerid, 217.8511, -98.4865, 1005.2578);
    ApplyAnimation(playerid, "benchpress", "gym_bp_celebrate", 4.1, true, false, false, false, 0, SYNC_NONE);
    return 1;
}
public OnPlayerCommandText(playerid, cmdtext[])
{
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerInterior(playerid, 0);
    LimparChat(playerid);
    return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
    return 1;
}

public OnVehicleSpawn(vehicleid)
{
    return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
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

public OnPlayerUpdate(playerid)
{
    return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    return 1;
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

public OnPlayerPickUpPickup(playerid, pickupid)
{
    return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
    return 1;
}

public OnPickupStreamIn(pickupid, playerid)
{
    return 1;
}

public OnPickupStreamOut(pickupid, playerid)
{
    return 1;
}

public OnPlayerPickupStreamIn(pickupid, playerid)
{
    return 1;
}

public OnPlayerPickupStreamOut(pickupid, playerid)
{
    return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, WEAPON:weaponid, bodypart)
{
    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
    return 1;
}

public OnScriptCash(playerid, amount, source)
{
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
    return 1;
}

stock LimparChat(playerid)
{
    for (new i = 0; i < 100; i++) SendClientMessage(playerid, -1, "");
    return 1;
}

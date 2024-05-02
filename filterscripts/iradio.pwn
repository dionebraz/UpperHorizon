#pragma tabsize 0
#include <open.mp>

public OnFilterScriptInit()
{
    return 1;
}

//-------------------------------------------------

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (IsABike(GetPlayerVehicleID(playerid) && GetPlayerVehicleSeat(playerid) == 0)) return 0;
    if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
        PlayAudioStreamForPlayer(playerid, "https://live.hunter.fm/hitsbrasil_high");
        LimparChat(playerid);
    }
    else if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
    {
        StopAudioStreamForPlayer(playerid);
    }
    return 0;
}

//-------------------------------------------------

public OnPlayerUpdate(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;
    if (IsPlayerNPC(playerid)) return 1;

    if (GetPlayerInterior(playerid) == 17)
    {
        if (IsPlayerInRangeOfPoint(playerid, 70.0, 489.5824, -14.7563, 1000.6797)) // alhambra middle
        {
            if (!GetPVarInt(playerid, "alhambra"))
            {
                SetPVarInt(playerid, "alhambra", 1);
                PlayAudioStreamForPlayer(playerid, "https://live.hunter.fm/80s_high", 480.9575, -3.5402, 1002.0781, 40.0, true);
                LimparChat(playerid);
            }
        }
    }
    else
    {
        if (GetPVarInt(playerid, "alhambra"))
        {
            DeletePVar(playerid, "alhambra");
            StopAudioStreamForPlayer(playerid);
        }
    }

    return 1;
}

//-------------------------------------------------

stock IsABike(vehicleid)
{
    new model = GetVehicleModel(vehicleid);
    if (model == 581 || model == 509 || model == 481 || model == 462 || model == 521 || model == 463 || model == 510 || model == 522 || model == 461 || model == 448 || model == 471 || model == 468 || model == 586)
    {
        return 1;
    }
    return 0;
}

stock LimparChat(playerid)
{
    for (new i = 0; i < 100; i++) SendClientMessage(playerid, -1, "");
    return 1;
}

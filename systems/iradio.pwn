#include <YSI-Includes\YSI_Coding\y_hooks>

hook OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
    if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
        PlayAudioStreamForPlayer(playerid, "https://live.hunter.fm/hitsbrasil_high", 0.0, 0.0, 0.0, 0.0, false);
    }
    else if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
    {
        StopAudioStreamForPlayer(playerid);
    }
    return 1;
}

stock IsABike(vehicleid)
{
    new model = GetVehicleModel(vehicleid);
    if (model == 581 || model == 509 || model == 481 || model == 462 || model == 521 || model == 463 || model == 510 || model == 522 || model == 461 || model == 448 || model == 471 || model == 468 || model == 586)
    {
        return 1;
    }
    return 0;
}

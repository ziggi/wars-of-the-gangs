/*
*	Created:			06.08.10
*	Author:				009
*	Description:		Маркеры на карте (на основе зон банд)
*/

#if defined _markers_included
	#endinput
#endif

#define _markers_included
#pragma library markers

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_MARKERS				MAX_GANG_ZONES
#define INVALID_MARKER_ID		INVALID_GANG_ZONE
#define MARKER_SIZE				15.0

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock CreateMarker(Float:X,Float:Y)
{
	return GangZoneCreate((X - MARKER_SIZE),(Y - MARKER_SIZE),(X + MARKER_SIZE),(Y + MARKER_SIZE));
}

stock DestroyMarker(markerid)
{
	return GangZoneDestroy(markerid);
}

stock ShowMarkerForPlayer(playerid,markerid,color)
{
	return GangZoneShowForPlayer(playerid,markerid,color);
}

stock ShowMarkerForAll(markerid,color)
{
	return GangZoneShowForAll(markerid,color);
}
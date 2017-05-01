/*
*	Created:			13.05.10
*	Author:				009
*	Description:		סענטללונ מבüוךעמג
*/

#if defined _streamer_objects_included
#endinput
#endif

#define _streamer_objects_included
#pragma library streamer_objects

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_STREAMED_OBJECTS	1500
#define OBJECTS_STREAMED_DIST	200.0

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	ObjectsStatus[MAX_STREAMED_OBJECTS char],
	ObjectsModel[MAX_STREAMED_OBJECTS],
	Float:ObjectsData[MAX_STREAMED_OBJECTS][6],
	PlayerObject[MAX_PLAYERS][MAX_STREAMED_OBJECTS],
	MinObjectId,
	ObjectsCount;

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock CreateStreamedObject(model,Float:X,Float:Y,Float:Z,Float:rX,Float:rY,Float:rZ)
{
	static id;
	id = MinObjectId;
	if(id >= MAX_STREAMED_OBJECTS)
	{
		print("[ERROR] Limit of streamed objects");
		return 0;
	}
	ObjectsStatus{id} = 1;
	ObjectsModel[id] = model;
	ObjectsData[id][0] = X;
	ObjectsData[id][1] = Y;
	ObjectsData[id][2] = Z;
	ObjectsData[id][3] = rX;
	ObjectsData[id][4] = rY;
	ObjectsData[id][5] = rZ;
	// free
	for(new i = 0;i < MAX_PLAYERS;i++) PlayerObject[i][id] = -1;
	// min object
	do MinObjectId++;
	while((MinObjectId < MAX_STREAMED_OBJECTS) && (ObjectsStatus{MinObjectId} == 1));
	// count
	if(ObjectsCount < MinObjectId) ObjectsCount = MinObjectId;
	// ret id
	return id;
}

stock DestroyStreamedObject(objectid)
{
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		
		if(PlayerObject[playerid][objectid] != -1) 
		{
			DestroyPlayerObject(playerid,PlayerObject[playerid][objectid]);
			PlayerObject[playerid][objectid] = -1;
		}
	}
	ObjectsStatus{objectid} = 0;
}

stock GetStreamedObjectPos(objectid,&Float:x,&Float:y,&Float:z)
{
	if(!ObjectsStatus{objectid}) return;
	
	x = ObjectsData[objectid][0];
	y = ObjectsData[objectid][1];
	z = ObjectsData[objectid][2];
}

stock PlayerObjectStream(playerid)
{
	for(new i = 0; i < ObjectsCount; i++)
	{
		if(ObjectsStatus{i} != 1) continue;
	
		if(PlayerObject[playerid][i] == -1)
		{
			if(IsPlayerInRangeOfPoint(playerid,OBJECTS_STREAMED_DIST,ObjectsData[i][0],ObjectsData[i][1],ObjectsData[i][2]))
			{
				PlayerObject[playerid][i] = CreatePlayerObject(playerid,ObjectsModel[i],ObjectsData[i][0],ObjectsData[i][1],ObjectsData[i][2],ObjectsData[i][3],ObjectsData[i][4],ObjectsData[i][5]);
			}
		}
		else if(!IsPlayerInRangeOfPoint(playerid,OBJECTS_STREAMED_DIST,ObjectsData[i][0],ObjectsData[i][1],ObjectsData[i][2]))
		{
			DestroyPlayerObject(playerid,PlayerObject[playerid][i]);
			PlayerObject[playerid][i] = -1;
		}
	}
}

// --------------------------------------------------
// forwards
// --------------------------------------------------
#if !defined ON_PLAYER_UPDATE_STREAMERS
forward Streamer_Objects();
#endif

// --------------------------------------------------
// publics
// --------------------------------------------------
#if !defined ON_PLAYER_UPDATE_STREAMERS
public Streamer_Objects()
{
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		
		PlayerObjectStream(playerid);
	}
}
#endif

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
StreamerObj_OnGameModeInit()
{
#if !defined ON_PLAYER_UPDATE_STREAMERS
	SetTimer("Streamer_Objects",1000,1);
#endif
	print("Streamer objects loaded.");
}

StreamerObj_OnPlayerConnect(playerid)
{
	for(new i = 0; i < ObjectsCount; i++) PlayerObject[playerid][i] = -1;
}

StreamerObj_OnPlayerDisconnect(playerid)
{
	for(new i = 0; i < ObjectsCount; i++)
	{
		if(PlayerObject[playerid]{i} != -1)
		{
			DestroyPlayerObject(playerid,PlayerObject[playerid][i]);
			PlayerObject[playerid][i] = -1;
		}
	}
}

#if defined ON_PLAYER_UPDATE_STREAMERS
StreamerObj_OnPlayerUpdate(playerid)
{
	PlayerObjectStream(playerid);
}
#endif
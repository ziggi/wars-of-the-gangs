/*
*	Created:			04.06.10
*	Author:				009
*	Description:		стриммер чекпоинтов
*/

#if defined _streamer_checkpoints_included
	#endinput
#endif

#define _streamer_checkpoints_included
#pragma library streamer_checkpoints

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_STREAMED_CHECKPOINTS	600
#define CHECKPOINTS_STREAMED_DIST	200.0
#if !defined GetDistanceBetweenCoords
	#define GetDistanceBetweenCoords(%1,%2,%3,%4,%5,%6) floatsqroot((%4 - %1)*(%4 - %1) + (%5 - %2)*(%5 - %2) + (%6 - %3)*(%6 - %3))
#endif

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	CheckpointsStatus[MAX_STREAMED_CHECKPOINTS char],
	Float:CheckpointsData[MAX_STREAMED_CHECKPOINTS][4],
	PlayerCurrentCheckpoint[MAX_PLAYERS],
	PlayerUseStream[MAX_PLAYERS char],
	MinCheckpointId,
	CheckpointsCount;

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock CreateStreamedCheckpoint(Float:X,Float:Y,Float:Z,Float:Size)
{
	static id;
	id = MinCheckpointId;
	if(id >= MAX_STREAMED_CHECKPOINTS)
	{
		print("[ERROR] Limit of streamed checkpoints");
		return 0;
	}
	CheckpointsStatus{id} = 1;
	CheckpointsData[id][0] = X;
	CheckpointsData[id][1] = Y;
	CheckpointsData[id][2] = Z;
	CheckpointsData[id][3] = Size;
	// min cp
	do MinCheckpointId++;
	while((MinCheckpointId < MAX_STREAMED_CHECKPOINTS) && (CheckpointsStatus{MinCheckpointId} == 1));
	// count
	if(CheckpointsCount < MinCheckpointId) CheckpointsCount = MinCheckpointId;
	// ret id
	return id;
}

stock DestroyStreamedCheckpoint(checkpointid)
{
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		
		if(PlayerCurrentCheckpoint[playerid] == checkpointid)
		{
			DisablePlayerCheckpoint(playerid);
			PlayerCurrentCheckpoint[playerid] = -1;
		}
	}
	CheckpointsStatus{checkpointid} = 0;
}

stock GetStreamedCheckpointPos(checkpointid,&Float:x,&Float:y,&Float:z)
{
	if(!CheckpointsStatus{checkpointid}) return;
	
	x = CheckpointsData[checkpointid][0];
	y = CheckpointsData[checkpointid][1];
	z = CheckpointsData[checkpointid][2];
}

stock TogglePlayerCheckpointStreamer(playerid,newstate)
{
	if(newstate) 
	{
		PlayerUseStream{playerid} = 1;
	}
	else
	{
		PlayerUseStream{playerid} = 0;
		if(PlayerCurrentCheckpoint[playerid] != -1)
		{
			DisablePlayerCheckpoint(playerid);
			PlayerCurrentCheckpoint[playerid] = -1;
		}
	}
}

stock PlayerCPStream(playerid)
{
	static 
		Float:dist,
		Float:c_dist,
		c_id,
		Float:pos[3];
	// null dist
	c_dist = 64000.0;
	c_id = -1;
	// my pos
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	// find range checkpoint
	for(new i = 0; i < CheckpointsCount; i++)
	{
		if(CheckpointsStatus{i} != 1) continue;
		
		dist = GetDistanceBetweenCoords(pos[0],pos[1],pos[2],CheckpointsData[i][0],CheckpointsData[i][1],CheckpointsData[i][2]);
		
		if(dist > CHECKPOINTS_STREAMED_DIST) continue;
		
		if(dist < c_dist)
		{
			c_dist = dist;
			c_id = i;
		}
	}
	// show range checkpoint
	if((c_id == -1) && (PlayerCurrentCheckpoint[playerid] != -1) || GetPlayerVirtualWorld(playerid) == STUNT_VIRTUAL_WORLD)
	{
		DisablePlayerCheckpoint(playerid);
		PlayerCurrentCheckpoint[playerid] = -1;
	}
	else if((c_id != -1) && (PlayerCurrentCheckpoint[playerid] != c_id))
	{
		DisablePlayerCheckpoint(playerid);
		SetPlayerCheckpoint(playerid,CheckpointsData[c_id][0],CheckpointsData[c_id][1],CheckpointsData[c_id][2],CheckpointsData[c_id][3]);
		PlayerCurrentCheckpoint[playerid] = c_id;
	}
}

// --------------------------------------------------
// forwards
// --------------------------------------------------
#if !defined ON_PLAYER_UPDATE_STREAMERS
forward Streamer_Checkpoints();
#endif
forward OnPlayerEnterStreamedCheckpoint(playerid,checkpointid);

// --------------------------------------------------
// publics
// --------------------------------------------------
#if !defined ON_PLAYER_UPDATE_STREAMERS
public Streamer_Checkpoints()
{
	for(new playerid = 0;playerid <= GetPlayersCount();playerid++)
	{
		if(!IsPlayerConnected(playerid)) continue;
		if(IsPlayerNPC(playerid)) continue;
		if(!PlayerUseStream{playerid}) continue;
		
		PlayerCPStream(playerid);
	}
}
#endif

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
StreamerCP_OnGameModeInit()
{
#if !defined ON_PLAYER_UPDATE_STREAMERS
	SetTimer("Streamer_Checkpoints",1000,1);
#endif
	print("Streamer checkpoints loaded.");
}

StreamerCP_OnPlayerConnect(playerid)
{
	PlayerCurrentCheckpoint[playerid] = -1;
	PlayerUseStream{playerid} = 1;
}

StreamerCP_OnPlayerDisconnect(playerid)
{
	PlayerCurrentCheckpoint[playerid] = -1;
}

StreamerCP_OnPlayerEnterCP(playerid)
{
	// check our checkpoint?
	if(PlayerCurrentCheckpoint[playerid] == -1) return;
	// in showed ?
	if(IsPlayerInRangeOfPoint(playerid,CheckpointsData[ PlayerCurrentCheckpoint[playerid] ][3],CheckpointsData[ PlayerCurrentCheckpoint[playerid] ][0],CheckpointsData[ PlayerCurrentCheckpoint[playerid] ][1],CheckpointsData[ PlayerCurrentCheckpoint[playerid] ][2]))
	{
		// call callback
		OnPlayerEnterStreamedCheckpoint(playerid,PlayerCurrentCheckpoint[playerid]);
	}
}

#if defined ON_PLAYER_UPDATE_STREAMERS
StreamerCP_OnPlayerUpdate(playerid)
{
	if(PlayerUseStream{playerid}) PlayerCPStream(playerid);
}
#endif
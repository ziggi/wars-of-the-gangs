/*
*	Created:			17.05.10
*	Author:				009
*	Description:		статусы выполнения квестов игроком
*/

#if defined _quest_included
	#endinput
#endif

#define _quest_included
#pragma library quest

// --------------------------------------------------
// defines
// --------------------------------------------------
#define INVALID_QUEST_ID		0
#define GetPlayerQuest(%1)		Quest_Player[%1]

// --------------------------------------------------
// statics
// --------------------------------------------------
static QuestsCount;

// --------------------------------------------------
// news
// --------------------------------------------------
new Quest_Player[MAX_PLAYERS]; // current quest

// --------------------------------------------------
// forwards
// --------------------------------------------------
forward OnPlayerChangeQuest(playerid,oldquest,newquest);

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock RegisterQuest()
{
	QuestsCount++;
	return QuestsCount;
}

stock SendClientMessageByQuest(questid,color,message[])
{
	for(new i = 0;i <= GetPlayersCount();i++)
	{
		if(!IsPlayerConnected(i)) continue;
		if(IsPlayerNPC(i)) continue;
		if(Quest_Player[i] != questid) continue;
		
		SendClientMessage(i,color,message);
	}
}

stock SetPlayerQuest(playerid,questid)
{
	OnPlayerChangeQuest(playerid,Quest_Player[playerid],questid);
	Quest_Player[playerid] = questid;
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Quest_OnPlayerConnect(playerid)
{
	Quest_Player[playerid] = INVALID_QUEST_ID;
}

Quest_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	Quest_Player[playerid] = INVALID_QUEST_ID;
}
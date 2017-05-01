/*
*	Created:			04.06.10
*	Author:				009
*	Description:		Админка.Уровни: Администратор,модератор, без привелегий
*/

#if defined _admin_included
	#endinput
#endif

#define _admin_included
#pragma library admin

// --------------------------------------------------
// includes
// --------------------------------------------------
#include "colors"

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_ADMIN_LEVEL_NAME	24
#define PRISON_COORDS			265.1273,77.6823,1001.0391
#define	PRISON_INTERIOR			6
#define ADMIN_LOG_FILE			"admin_log.txt"
// check functions
#if !defined strtok
	#error strtok not found in script.
#endif
#if !defined oGetPlayerMoney
	#error oGetPlayerMoney not found in script.
#endif
#if !defined oGivePlayerMoney
	#error oGivePlayerMoney not found in script.
#endif
#if !defined oGetPlayerName
	#error oGetPlayerName not found in script.
#endif
#if !defined IsValidSkin
	#error IsValidSkin not found in script.
#endif
#if !defined SetPlayerXP
	#error SetPlayerXP not found in script.
#endif
#if !defined CalculateXPForLevel
	#error CalculateXPForLevel not found in script.
#endif

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	ADMIN_LEVEL_NONE,
	ADMIN_LEVEL_MODER,
	ADMIN_LEVEL_ADMIN
};
enum
{
	ADMIN_CMDLIST_CMD,
	ADMIN_SSAY_CMD,
	ADMIN_BOOM_CMD,
	ADMIN_SETLVL_CMD,
	ADMIN_DISARM_CMD,
	ADMIN_REMMONEY_CMD,
	ADMIN_GIVEMONEY_CMD,
	ADMIN_SETSTATUS_CMD,
	ADMIN_BAN_CMD,
	ADMIN_STATUSES_CMD,
	ADMIN_SAY_CMD,
	ADMIN_SETSKIN_CMD,
	ADMIN_KICK_CMD,
	ADMIN_AKILL_CMD,
	ADMIN_JAIL_CMD,
	ADMIN_UNJAIL_CMD,
	ADMIN_PARALYZE_CMD,
	ADMIN_DEPARALYZE_CMD,
	ADMIN_MUTE_CMD,
	ADMIN_UNMUTE_CMD,
	ADMIN_TELETO_CMD,
	ADMIN_TELEHERE_CMD,
	ADMIN_TELEPORT_CMD,
	ADMIN_UPGRADEPOINT_CMD
};
enum
{
	ADMIN_REPORT_TEXT
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static AdminLevelName[][MAX_ADMIN_LEVEL_NAME] =
{
	"Без привелегий",
	"Модератор",
	"Администратор"
};

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock AdminLog(fstring[], {Float, _}:...)
{
	// open file
	new File:al = fopen(MODE_DIR ADMIN_LOG_FILE,io_append);
	if(al)
	{
		// timestamp check
		#if defined TIMESTAMP_IN_LOG
			new time[3],date[3];
			gettime(time[0],time[1],time[2]);// &hour=0, &minute=0, &second=0
			getdate(date[0],date[1],date[2]);// &year=0, &month=0, &day=0
			format(stmp,sizeof(stmp),"[%02d:%02d:%02d-%02d:%02d:%04d]",time[0],time[1],time[2],date[2],date[1],date[0]);
			fwrite(al,stmp);
		#endif
		// used Y_Less's code
		#define BYTES_PER_CELL (cellbits / 8)
		// This is the number of parameters which are not variable that are passed
		// to this function (i.e. the number of named parameters).
		static const
			STATIC_ARGS = 1;
		// Get the number of variable arguments.
		new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
		if(n)
		{
			new
				message[128],
				arg_start,
				arg_end;

			// Load the real address of the last static parameter. Do this by
			// loading the address of the last known static parameter and then
			// adding the value of [FRM].
			#emit CONST.alt			fstring
			#emit LCTRL				5
			#emit ADD
			#emit STOR.S.pri		arg_start

			// Load the address of the last variable parameter. Do this by adding
			// the number of variable parameters on the value just loaded.
			#emit LOAD.S.alt		n
			#emit ADD
			#emit STOR.S.pri		arg_end

			// Push the variable arguments. This is done by loading the value of
			// each one in reverse order and pushing them. I'd love to be able to
			// rewrite this to use the values of pri and alt for comparison,
			// instead of having to constantly load and reload two variables.
			do
			{
				#emit LOAD.I
				#emit PUSH.pri
				arg_end -= BYTES_PER_CELL;
				#emit LOAD.S.pri      arg_end
			}
			while (arg_end > arg_start);

			// Push the static format parameters.
			#emit PUSH.S			fstring
			#emit PUSH.C			128
			#emit PUSH.ADR			message

			// Now push the number of arguments passed to format, including both
			// static and variable ones and call the function.
			n += BYTES_PER_CELL * 3;
			#emit PUSH.S			n
			#emit SYSREQ.C			format

			// Remove all data, including the return value, from the stack.
			n += BYTES_PER_CELL;
			#emit LCTRL				4
			#emit LOAD.S.alt		n
			#emit ADD
			#emit SCTRL				4

			fwrite(al,message);
		}
		else fwrite(al,fstring);
		// undef
		#undef BYTES_PER_CELL
		// write end
		fwrite(al,"\r\n");
		// close
		fclose(al);
		return 1;
	}
	else 
	{
		print("[ERROR] cant open admin log file");
		return 0;
	}
}

stock oGetPlayerAdminLevelName(playerid)
{
	static name[MAX_ADMIN_LEVEL_NAME];
	name = AdminLevelName[  GetPVarInt(playerid,"AdminLevel") ];
	return name;
}

// --------------------------------------------------
// forwards
// --------------------------------------------------


// --------------------------------------------------
// publics
// --------------------------------------------------


// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Admin_OnGameModeInit()
{
	// data
#if defined PRINT_STATS_DATA
	printf("Admin levels: %d",sizeof(AdminLevelName));
#endif
	print("Admin loaded.");
}

Admin_Text(playerid,textid,string[])
{
	switch(textid)
	{
		case ADMIN_REPORT_TEXT:
		{
			// admin log
			AdminLog("[REPORT] %s - %s",oGetPlayerName(playerid),string);
			// send
			format(stmp,sizeof(stmp),"Report from %s: %s",oGetPlayerName(playerid),string);
			new count;
			for(new id = 0; id <= GetPlayersCount();id++)
			{
				if(!IsPlayerConnected(id)) continue;
				if(IsPlayerNPC(id)) continue;
				if((GetPVarInt(id,"AdminLevel") == ADMIN_LEVEL_NONE) && (IsPlayerAdmin(id) == 0)) continue;

				SendClientMessage(id,COLOR_RED,stmp);
				count++;
			}
			if(count == 0) SendClientMessage(playerid,COLOR_WHITE,"На данный момент на сервере нет администрации");
			else 
			{
				format(stmp,sizeof(stmp),"Cообщение \"%s\" отправлено администрации сервера",string);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
			}
			return 1;
		}
	}
	return 0;
}

Admin_Command(playerid,cmdid,params[])
{
	// check for none priveleges
	if((GetPVarInt(playerid,"AdminLevel") == ADMIN_LEVEL_NONE) && (!IsPlayerAdmin(playerid))) return 0;
	// parse cmd
	new idx;
	switch(cmdid)
	{
		case ADMIN_CMDLIST_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - cmdlist %s",oGetPlayerName(playerid),params);
			// cmd
			SendClientMessage(playerid, COLOR_GREEN,"Admin commands (R - RCON, A - Admin, M - Moder)");
			if(IsPlayerAdmin(playerid))
			{
				SendClientMessage(playerid, COLOR_WHITE, "R: '/ssay <text>', '/boom <ID>', '/setlvl <ID> <lvl>'");
				SendClientMessage(playerid, COLOR_WHITE, "R: '/disarm <ID>', '/remmoney <ID>', '/agivemoney <ID> <amount>'");
			}
			if((GetPVarInt(playerid,"AdminLevel") == ADMIN_LEVEL_ADMIN) || IsPlayerAdmin(playerid))
			{
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "A: '/setstatus <ID> <status>', '/ban <ID> <reason>', '/statuses'");
			}
			SendClientMessage(playerid, COLOR_LIGHTRED, "M: '/say <text>', '/setskin <ID> <skin>', '/kick <ID> <reason>', '/akill <ID> <reason>'");
			SendClientMessage(playerid, COLOR_LIGHTRED, "M: '/jail <ID> <reason>', '/unjail <ID>', '/paralyze <ID> <reason>', '/deparalyze <ID>'");
			SendClientMessage(playerid, COLOR_LIGHTRED, "M: '/tele-to <ID>', '/tele-here <ID>', '/mute <ID> <reason>', '/unmute <ID>', '/upoint <ID> <count>', '/freehouse'");
			return 1;
		}
		case ADMIN_SSAY_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - ssay %s",oGetPlayerName(playerid),params);
			// cmd
			
			format(stmp,sizeof(stmp),"SERVER: %s",params);
			SendClientMessageToAll(COLOR_WHITE,stmp);
			return 1;
		}
		case ADMIN_BOOM_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - boom %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx)),
				Float:X,
				Float:Y,
				Float:Z;
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			GetPlayerPos(id,X,Y,Z);
			CreateExplosion(X,Y,(Z + 2.0),1,30.0);
			
			format(stmp,sizeof(stmp),"%s boom!",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_WHITE,stmp);
			return 1;
		}
		case ADMIN_SETLVL_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - setlvl %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx)),
				lvl = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"Администратор %s установил вам %d уровень",oGetPlayerName(playerid),lvl);
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы установили %s %d уровень",oGetPlayerName(id),lvl);
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			
			SetPlayerLevel(id,lvl);
			return 1;
		}
		case ADMIN_DISARM_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - disarm %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
		
			ResetPlayerWeapons(id);

			format(stmp,sizeof(stmp),"Администратор %s разоружил вас",oGetPlayerName(playerid));
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы разоружили %s",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			return 1;
		}
		case ADMIN_REMMONEY_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - remmoney %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
		
			ResetPlayerMoney(id);

			format(stmp,sizeof(stmp),"Администратор %s лишил вас денег",oGetPlayerName(playerid));
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы лишили %s денег",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			return 1;
		}
		case ADMIN_GIVEMONEY_CMD:
		{
			if(!IsPlayerAdmin(playerid)) return 0;
			// admin log
			AdminLog("[CMD] %s - givemoney %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx)),
				amount = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
		
			oGivePlayerMoney(id,amount);

			format(stmp,sizeof(stmp),"Администратор %s дал вам %d$",oGetPlayerName(playerid),amount);
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы дали %d$ %s",amount,oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			return 1;
		}
		case ADMIN_SETSTATUS_CMD:
		{
			if((GetPVarInt(playerid,"AdminLevel") != ADMIN_LEVEL_ADMIN) && (!IsPlayerAdmin(playerid))) return 0;
			// admin log
			AdminLog("[CMD] %s - setstatus %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx)),
				sid = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			if((id == playerid) && (!IsPlayerAdmin(playerid)))
			{
				SendClientMessage(playerid,COLOR_WHITE,"Вы не можете изменить свой статус");
				return 1;			
			}
			
			if((!IsPlayerAdmin(playerid)) && (sid > ADMIN_LEVEL_MODER))
			{
				format(stmp,sizeof(stmp),"Только RCON администратор может назначать статусы выше уровня '%s'",AdminLevelName[ADMIN_LEVEL_MODER]);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s изменил ваш статус, теперь вы %s",oGetPlayerName(playerid),AdminLevelName[sid]);
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы изменили статус %s на %s",oGetPlayerName(id),AdminLevelName[sid]);
			SendClientMessage(playerid,COLOR_GREEN,stmp);
			
			SetPVarInt(id,"AdminLevel",sid);
			return 1;
		}
		case ADMIN_BAN_CMD:
		{
			if((GetPVarInt(playerid,"AdminLevel") != ADMIN_LEVEL_ADMIN) && (!IsPlayerAdmin(playerid))) return 0;
			// admin log
			AdminLog("[CMD] %s - ban %s",oGetPlayerName(playerid),params);
			// cmd
		
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			new ip[16];
			GetPlayerIp(id,ip,sizeof(ip));
			
			format(stmp,sizeof(stmp),"%s забанен администратором %s по причине: %s",oGetPlayerName(id),oGetPlayerName(playerid),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s(%s) banned",oGetPlayerName(id),ip);
			SendClientMessage(playerid,COLOR_GREEN,stmp);
						
			BanEx(id,params[idx]);
			return 1;
		}
		case ADMIN_STATUSES_CMD:
		{
			if((GetPVarInt(playerid,"AdminLevel") != ADMIN_LEVEL_ADMIN) && (!IsPlayerAdmin(playerid))) return 0;
			// admin log
			AdminLog("[CMD] %s - statuses %s",oGetPlayerName(playerid),params);
			// cmd
		
			for(new i = 0;i < sizeof(AdminLevelName);i++) 
			{
				format(stmp,sizeof(stmp),"%d - '%s'",i,AdminLevelName[i]);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
			}
			return 1;
		}
		case ADMIN_SAY_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - say %s",oGetPlayerName(playerid),params);
			// cmd
			if(!IsPlayerAdmin(playerid))
			{
				format(stmp,sizeof(stmp),"%s: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],params);
				SendClientMessageToAll(COLOR_BLUE,stmp);
			}
			else 
			{
				format(stmp,sizeof(stmp),"Administrator: %s",params);
				SendClientMessageToAll(COLOR_BLUE,stmp);
			}
			return 1;
		}
		case ADMIN_SETSKIN_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - setskin %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx)),
				sid = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			if(!IsValidSkin(sid))
			{
				format(stmp,sizeof(stmp),"%d invalid skin id",sid);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s изменил ваш скин на %d",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),sid);
			SendClientMessage(id,COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"Вы изменили скин %s с %d на %d",oGetPlayerName(id),GetPlayerSkin(id),sid);
			SendClientMessage(playerid,COLOR_GREEN,stmp);
						
			SetPlayerSkin(id,sid);
			return 1;
		}
		case ADMIN_KICK_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - kick %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			new ip[16];
			GetPlayerIp(id,ip,sizeof(ip));
			
			format(stmp,sizeof(stmp),"%s %s кикнул %s по причине: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s(%s) kicked",oGetPlayerName(id),ip);
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			Kick(id);
			return 1;
		}
		case ADMIN_AKILL_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - akill %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s убил %s по причине: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s die",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			SetPlayerHealth(id,0.0);
			return 1;
		}
		case ADMIN_JAIL_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - jail %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s посадил в тюрьму %s по причине: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s jailed",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			SetPVarInt(id,"IsJailed",1);
			return 1;
		}
		case ADMIN_UNJAIL_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - unjail %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s освободил из тюрьмы %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id));
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s unjailed",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			SetPVarInt(id,"IsJailed",0);
			return 1;
		}
		case ADMIN_PARALYZE_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - paralyze %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s заморозил %s по причине: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s paralyzed",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			TogglePlayerControllable(id,false);
			return 1;
		}
		case ADMIN_DEPARALYZE_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - deparalyze %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s разморозил %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id));
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s deparalyzed",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			TogglePlayerControllable(id,true);
			return 1;
		}
		case ADMIN_MUTE_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - mute %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s запретил говорить %s по причине: %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id),params[idx]);
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s muted",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			SetPVarInt(id,"IsMuted",1);
			return 1;
		}
		case ADMIN_UNMUTE_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - unmute %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			format(stmp,sizeof(stmp),"%s %s разрешил говорить %s",AdminLevelName[ GetPVarInt(playerid,"AdminLevel") ],oGetPlayerName(playerid),oGetPlayerName(id));
			SendClientMessageToAll(COLOR_GREEN,stmp);
			format(stmp,sizeof(stmp),"%s unmuted",oGetPlayerName(id));
			SendClientMessage(playerid,COLOR_GREEN,stmp);

			SetPVarInt(id,"IsMuted",0);
			return 1;
		}
		case ADMIN_TELETO_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - teleto %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx)),
				Float:X,
				Float:Y,
				Float:Z;
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			GetPlayerPos(id,X,Y,Z);
			if(!IsPlayerInAnyVehicle(playerid)) SetPlayerPos(playerid,(X + random(3) - random(3)),(Y + random(3) - random(3)),Z);
			else SetVehiclePos(GetPlayerVehicleID(playerid),(X + random(3) - random(3)),(Y + random(3) - random(3)),Z);
			return 1;
		}
		case ADMIN_TELEHERE_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - telehere %s",oGetPlayerName(playerid),params);
			// cmd
			new id = strval(strtok(params,idx)),
				Float:X,
				Float:Y,
				Float:Z;
			
			if(!IsPlayerConnected(id))
			{
				format(stmp,sizeof(stmp),"%d offline",id);
				SendClientMessage(playerid,COLOR_WHITE,stmp);
				return 1;
			}
			
			GetPlayerPos(playerid,X,Y,Z);
			if(!IsPlayerInAnyVehicle(id)) SetPlayerPos(id,(X + random(3) - random(3)),(Y + random(3) - random(3)),Z);
			else SetVehiclePos(GetPlayerVehicleID(id),(X + random(3) - random(3)),(Y + random(3) - random(3)),Z);
			return 1;
		}
		case ADMIN_TELEPORT_CMD:
		{
			// admin log
			AdminLog("[CMD] %s - teleport %s",oGetPlayerName(playerid),params);
			// cmd
			new Float:X = floatstr(strtok(params,idx,',')),
				Float:Y = floatstr(strtok(params,idx,',')),
				Float:Z = floatstr(strtok(params,idx,','));
			
			if(!IsPlayerInAnyVehicle(playerid)) SetPlayerPos(playerid,X,Y,Z);
			else SetVehiclePos(GetPlayerVehicleID(playerid),X,Y,Z);
			return 1;
		}
		case ADMIN_UPGRADEPOINT_CMD:
		{
			new id = strval(strtok(params,idx)),
				count = strval(strtok(params,idx));
			
			if(!IsPlayerConnected(id))
				return SendClientMessage(playerid,COLOR_RED,"Этот игрок оффлайн");
			
			if(count < 0)
				return SendClientMessage(playerid,COLOR_RED,"Не корректное количество очков");
			
			SetPVarInt(id,"UpgradesPoints",GetPVarInt(id,"UpgradesPoints") + count);
			SendClientMessage(id,COLOR_GREEN,"Администрация дала вам несколько UpgradePoint");
			SendClientMessage(playerid,COLOR_RED,"Всё прошло успешно.");
			return 1;
		}
	}
	return 0;
}

Admin_OnPlayerUpdate(playerid)
{
	if(GetPVarInt(playerid,"IsJailed"))
	{
		if(!IsPlayerInRangeOfPoint(playerid,10.0,PRISON_COORDS))
		{
			SetPlayerPos(playerid,PRISON_COORDS);
			SetPlayerInterior(playerid,PRISON_INTERIOR);
		}
	}
}

Admin_OnSavePlayerData(playerid,File:datafile,reason)
{
	#pragma unused reason
	format(stmp,sizeof(stmp),"AdminLevel=%d\r\n",GetPVarInt(playerid,"AdminLevel"));
	fwrite(datafile,stmp);
	format(stmp,sizeof(stmp),"IsJailed=%d\r\n",GetPVarInt(playerid,"IsJailed"));
	fwrite(datafile,stmp);
	format(stmp,sizeof(stmp),"IsMuted=%d\r\n",GetPVarInt(playerid,"IsMuted"));
	fwrite(datafile,stmp);
}

Admin_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	if(!strcmp(datastr,"AdminLevel",true,separatorpos)) 
	{
		SetPVarInt(playerid,"AdminLevel",strval(datastr[separatorpos + 1]));
	}
	if(!strcmp(datastr,"IsJailed",true,separatorpos)) 
	{
		SetPVarInt(playerid,"IsJailed",strval(datastr[separatorpos + 1]));
	}
	if(!strcmp(datastr,"IsMuted",true,separatorpos)) 
	{
		SetPVarInt(playerid,"IsMuted",strval(datastr[separatorpos + 1]));
	}
}
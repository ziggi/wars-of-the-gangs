/*
*	Created:			23.06.10
*	Author:				009
*	Description:		Античиты
*/

#if defined _protect_included
	#endinput
#endif

#define _protect_included
#pragma library protect

// --------------------------------------------------
// defines
// --------------------------------------------------
#define PROTECT_LOG_FILE		"protect_log.txt"
#define MAX_WEAPON_SLOTS		12
#define MAX_CHEAT_MONEY			500
#define MAX_NOOB_KILL_REASONS	2
#define MAX_MONEY				2_000_000_000
// check functions
#if !defined GetWeaponSlot
	#error GetWeaponSlot not found in script.
#endif

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	/*Float:old_coord[MAX_PLAYERS][3],*/
	NoobKillReasons[MAX_PLAYERS char];

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock ProtectLog(fstring[], {Float, _}:...)
{
	// open file
	new File:al = fopen(MODE_DIR PROTECT_LOG_FILE,io_append);
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
			print(message);
		}
		else 
		{
			fwrite(al,fstring);
			print(fstring);
		}
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
		print("[ERROR] cant open protect log file");
		return 0;
	}
}
// weapon
stock oGivePlayerWeapon(playerid,weaponid,ammo)
{
	static tmp;
	static str[20];
	tmp	= GetWeaponSlot(weaponid);
	format(str,sizeof(str),"AC_SlotWeapon_%d",tmp);
	SetPVarInt(playerid,str,weaponid);
	format(str,sizeof(str),"AC_SlotAmmo_%d",tmp);
	SetPVarInt(playerid,str,GetPVarInt(playerid,str) + ammo);
	GivePlayerWeapon(playerid,weaponid,ammo);
}

stock oGetPlayerWeaponData(playerid,slot,&weapons,&ammo)
{
	static str[20];
	format(str,sizeof(str),"AC_SlotWeapon_%d",slot);
	weapons = GetPVarInt(playerid,str);
	format(str,sizeof(str),"AC_SlotAmmo_%d",slot);
	ammo = GetPVarInt(playerid,str);	
}

stock oResetPlayerWeapons(playerid)
{
	static str[20];
	for(new i = 0;i < MAX_WEAPON_SLOTS;i++)
	{
		format(str,sizeof(str),"AC_SlotWeapon_%d",i);
		SetPVarInt(playerid,str,0);
		format(str,sizeof(str),"AC_SlotAmmo_%d",i);
		SetPVarInt(playerid,str,0);
	}
	ResetPlayerWeapons(playerid);
}

stock GivePlayerOwnedWeapons(playerid)
{
	static str[20],str2[20];
	for(new i=0;i<MAX_WEAPON_SLOTS;i++)
	{
		format(str,sizeof(str),"AC_SlotWeapon_%d",i);
		format(str2,sizeof(str2),"AC_SlotAmmo_%d",i);
		GivePlayerWeapon(playerid,GetPVarInt(playerid,str),GetPVarInt(playerid,str2));
	}
}
// money
stock oResetPlayerMoney(playerid)
{
	SetPVarInt(playerid,"AC_Money",0);
	ResetPlayerMoney(playerid);
}

stock oGivePlayerMoney(playerid,money)
{
	if(GetPVarInt(playerid,"AC_Money") >= MAX_MONEY) return 0;
	if(GetPVarInt(playerid,"AC_Money") + money > MAX_MONEY)
	{
		money = MAX_MONEY - GetPVarInt(playerid,"AC_Money");
		SendClientMessage(playerid,COLOR_BLUE,"У вас максимальное количество денег!");
	}
	SetPVarInt(playerid,"AC_Money",GetPVarInt(playerid,"AC_Money") + money);
	GivePlayerMoney(playerid,money);
	return 1;
}

stock oSetPlayerMoney(playerid,money)
{
	if(money > MAX_MONEY) return 0;
	SetPVarInt(playerid,"AC_Money",money);
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid,money);
	return 1;
}

stock oGetPlayerMoney(playerid)
{
	return GetPVarInt(playerid,"AC_Money");
}
// health
stock oSetPlayerHealth(playerid,Float:health)
{
	SetPVarFloat(playerid,"AC_Health",health);
	SetPlayerHealth(playerid,health);
}

stock Float:oGetPlayerHealth(playerid)
{
	return GetPVarFloat(playerid,"AC_Health");
}
// armour
stock oSetPlayerArmour(playerid,Float:armour)
{
	SetPVarFloat(playerid,"AC_Armour",armour);
	SetPlayerArmour(playerid,armour);
}

stock Float:oGetPlayerArmour(playerid)
{
	return GetPVarFloat(playerid,"AC_Armour");
}
// special actions
stock oSetPlayerSpecialAction(playerid,actionid)
{
	SetPVarInt(playerid,"AC_SpecialAction",actionid);
	SetPlayerSpecialAction(playerid,actionid);
}

stock oGetPlayerSpecialAction(playerid)
{
	return GetPVarInt(playerid,"AC_SpecialAction");
}

// --------------------------------------------------
// Obligatory functions
// --------------------------------------------------
Protect_OnGameModeInit()
{
	print("Protect loaded.");
}

Protect_OnPlayerUpdate(playerid)
{
	static tmp[5];
	static Float:float_tmp[2];
	// anti weapon cheat
	/*if((tmp[0] = GetPlayerWeapon(playerid)) != 0)
	{
		if(GetPlayerQuest(playerid) == INVALID_QUEST_ID)
		{
			// get slot
			tmp[0]= GetWeaponSlot(tmp[0]);
			// get needed data
			oGetPlayerWeaponData(playerid,tmp[0],tmp[1],tmp[2]);
			// get current data
			GetPlayerWeaponData(playerid,tmp[0],tmp[3],tmp[4]);
			// check weapon
			if(tmp[1] != tmp[3])
			{
				static ip[16];
				GetPlayerIp(playerid,ip,sizeof(ip));
				ProtectLog("%s (%s) has been banned.reason: weapon cheat (%d != %d weapon)",oGetPlayerName(playerid),ip,tmp[1],tmp[3]);
				//Ban(playerid);
				return 0;
			}
			// check ammo
			if(tmp[2] < tmp[4])
			{
				static ip[16];
				GetPlayerIp(playerid,ip,sizeof(ip));
				ProtectLog("%s (%s) has been banned.reason: weapon cheat (%d < %d ammo)",oGetPlayerName(playerid),ip,tmp[2],tmp[4]);
				//Ban(playerid);
				return 0;
			}
		}
	}
	// anti money cheat
	tmp[0] = oGetPlayerMoney(playerid);
	if(GetPlayerMoney(playerid) > tmp[0])
	{
		if((GetPlayerMoney(playerid) - tmp[0]) > MAX_CHEAT_MONEY)
		{
			static ip[16];
			GetPlayerIp(playerid,ip,sizeof(ip));
			ProtectLog("%s (%s) has been banned.reason: money cheat (%d > %d)",oGetPlayerName(playerid),ip,GetPlayerMoney(playerid),tmp[0]);
			//Ban(playerid);
			return 0;
		}
		else oSetPlayerMoney(playerid,tmp[0]);
	}
	if(GetPlayerMoney(playerid) < tmp[0]) oSetPlayerMoney(playerid,GetPlayerMoney(playerid));
	// anti health cheat
	GetPlayerHealth(playerid,float_tmp[0]);
	float_tmp[1] = oGetPlayerHealth(playerid);
	if(float_tmp[0] > float_tmp[1])
	{
		static ip[16];
		GetPlayerIp(playerid,ip,sizeof(ip));
		ProtectLog("%s (%s) has been banned.reason: health cheat (%f > %f)",oGetPlayerName(playerid),ip,float_tmp[0],float_tmp[1]);
		//Ban(playerid);
		return 0;
	}
	if(float_tmp[0] < float_tmp[1])
	{
		// anti noob kill
		if(IsPlayerInRangeOfPoint(playerid,0.5,old_coord[playerid][0],old_coord[playerid][1],old_coord[playerid][2]))
		{
			GetPlayerKeys(playerid,tmp[0],tmp[1],tmp[2]);
			if((tmp[0] == 0) && (tmp[1] == 0) && (tmp[2] == 0)) NoobKillReasons{playerid}++;
		}
		else
		{
			NoobKillReasons{playerid} = 0;
		}
		oSetPlayerHealth(playerid,float_tmp[0]);
		GetPlayerPos(playerid,old_coord[playerid][0],old_coord[playerid][1],old_coord[playerid][2]);
	}
	// anti armour cheat
	GetPlayerArmour(playerid,float_tmp[0]);
	float_tmp[1] = oGetPlayerArmour(playerid);
	if(float_tmp[0] > float_tmp[1])
	{
		static ip[16];
		GetPlayerIp(playerid,ip,sizeof(ip));
		ProtectLog("%s (%s) has been banned.reason: armour cheat (%f > %f)",oGetPlayerName(playerid),ip,float_tmp[0],float_tmp[1]);
		//Ban(playerid);
		return 0;
	}
	if(float_tmp[0] < float_tmp[1]) oSetPlayerArmour(playerid,float_tmp[0]);
	*/
	tmp[0] = oGetPlayerMoney(playerid);
	tmp[1] = GetPlayerMoney(playerid);
	if(tmp[1] > tmp[0]) oSetPlayerMoney(playerid,tmp[0]);
	else if(tmp[1] < tmp[0]) oSetPlayerMoney(playerid,tmp[1]);
	
	GetPlayerHealth(playerid,float_tmp[0]);
	float_tmp[1] = oGetPlayerHealth(playerid);
	if(float_tmp[0] > float_tmp[1]) oSetPlayerHealth(playerid,float_tmp[1]);
	else if(float_tmp[0] < float_tmp[1]) oSetPlayerHealth(playerid,float_tmp[0]);
	
	GetPlayerArmour(playerid,float_tmp[0]);
	float_tmp[1] = oGetPlayerArmour(playerid);
	if(float_tmp[0] > float_tmp[1]) oSetPlayerArmour(playerid,float_tmp[1]);
	else if(float_tmp[0] < float_tmp[1]) oSetPlayerArmour(playerid,float_tmp[0]);
	return 1;
}

Protect_OPSAC(playerid,old_special_action,new_special_action) // OnPlayerSpecialActionChange
{
	if(new_special_action == SPECIAL_ACTION_USEJETPACK)
	{
		if(oGetPlayerSpecialAction(playerid) != SPECIAL_ACTION_USEJETPACK)
		{
			static ip[16];
			GetPlayerIp(playerid,ip,sizeof(ip));
			ProtectLog("%s (%s) has been banned.reason: jetpack cheat",oGetPlayerName(playerid),ip);
			//Ban(playerid);
		}
	}
	if(old_special_action == SPECIAL_ACTION_USEJETPACK)
	{
		oSetPlayerSpecialAction(playerid,new_special_action);
	}
}

Protect_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason
	
	if(NoobKillReasons{playerid} > MAX_NOOB_KILL_REASONS)
	{
		static ip[16];
		GetPlayerIp(killerid,ip,sizeof(ip));
		ProtectLog("%s (%s) has been kicked.reason: noob kill",oGetPlayerName(killerid),ip);
		//Kick(killerid);
	}
	NoobKillReasons{playerid} = 0;
	return 1;
}
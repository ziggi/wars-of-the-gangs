/*
*	Created:			04.06.10
*	Author:				009
*	Description:		Банки. Положить деньги на счёт, снять деньги.
*/

#if defined _banks_included
	#endinput
#endif

#define _banks_included
#pragma library banks

// --------------------------------------------------
// defines
// --------------------------------------------------
#define BANKS_DIR				"banks/"
#define BANKS_MASK_FILE			"bank_%d.txt"
#define BANK_PICKUP				1274
#define BANKS_LABEL_COLOR		0x331166FF
#define BANK_ICON				52
// check functions
#if !defined CheckFileLine
		#error CheckFileLine not found in script.
#endif
#if !defined strtok
	#error strtok not found in script.
#endif
#if !defined oGetPlayerMoney
	#error oGetPlayerMoney not found in script.
#endif
#if !defined ChangeStatByNameInt
	#error ChangeStatByNameInt not found in script.
#endif
#if !defined oGivePlayerMoney
	#error oGivePlayerMoney not found in script.
#endif
#if !defined oGetPlayerName
	#error oGivePlayerMoney not found in script.
#endif


// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	BANKS_HELP_CMD,
	BANKS_WITHDRAW_CMD,
	BANKS_DEPOSITE_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------
static 
	PickupsLink[MAX_PICKUPS] = {-1,...},
	PlayerBank[MAX_PLAYERS];

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadBankFromFile(filename[],bankid)
{
	new File:df = fopen(filename,io_read),
		k,
		c,
		name[24];
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// parse
			if((k = strfind(stmp,"=")) == -1) continue;
			
			if(!strcmp(stmp,"Coords",true,k))
			{
				c = 0;
				ftmp[0] = floatstr(strtok(stmp[k + 1],c,','));
				ftmp[1] = floatstr(strtok(stmp[k + 1],c,','));
				ftmp[2] = floatstr(strtok(stmp[k + 1],c,','));
				continue;
			}
			if(!strcmp(stmp,"Name",true,k))
			{
				strcat(name,stmp[k + 1]);
				continue;
			}
		}
		fclose(df);
		
		// create pickup
		k = CreatePickup(BANK_PICKUP,42,ftmp[0],ftmp[1],ftmp[2],0);
		PickupsLink[k] = bankid;
		// create 3D text
		Create3DTextLabel(name,BANKS_LABEL_COLOR,ftmp[0],ftmp[1],(ftmp[2] + 1.0),50.0,0,1);
		// create icon
#if defined _streamer_icons_included
		CreateStreamedIcon(BANK_ICON,0x0,ftmp[0],ftmp[1],ftmp[2]);
#endif
	}
}

stock GetBankInfo(bankid,name[],&Float:x,&Float:y,&Float:z)
{
	format(stmp,sizeof(stmp),MODE_DIR BANKS_DIR BANKS_MASK_FILE,bankid);
	if(fexist(stmp))
	{
		new File:df = fopen(stmp,io_read),
			k,
			c;
		if(df)
		{
			while(fread(df,stmp))
			{
				if(!CheckFileLine(stmp)) continue;
				// parse
				if((k = strfind(stmp,"=")) == -1) continue;
			
				if(!strcmp(stmp,"Coords",true,k))
				{
					c = 0;
					x = floatstr(strtok(stmp[k + 1],c,','));
					y = floatstr(strtok(stmp[k + 1],c,','));
					z = floatstr(strtok(stmp[k + 1],c,','));
					continue;
				}
				if(!strcmp(stmp,"Name",true,k))
				{
					strcat(name,stmp[k + 1],24);
					continue;
				}
			}
			fclose(df);
			return 1;		
		}
	}
	return 0;
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
Banks_OnGameModeInit()
{
	// load all banks
	new i = 0;
	format(stmp,sizeof(stmp),MODE_DIR BANKS_DIR BANKS_MASK_FILE,i);
	while(fexist(stmp))
	{
		LoadBankFromFile(stmp,i);
		// up
		i++;
		// again
		format(stmp,sizeof(stmp),MODE_DIR BANKS_DIR BANKS_MASK_FILE,i);
	}	
	// data
#if defined PRINT_STATS_DATA
	printf("Banks: %d",i);
#endif
	print("Banks loaded.");
}

Banks_OnPlayerPickUpPickup(playerid,pickupid)
{
	if(PickupsLink[pickupid] == -1) return;
	
	PlayerBank[playerid] = PickupsLink[pickupid];
}

Banks_Command(playerid,cmdid,params[])
{
	new idx;
	switch(cmdid)
	{
		case BANKS_HELP_CMD:
		{
			ShowPlayerDialog(playerid,DIALOG_NONE_ACTION,DIALOG_STYLE_MSGBOX,MODE_NAME " -> Банки","Вы можете положить свои деньги в банк\nПри смерти деньги из банка не будут забираться\nКоманды:\n/deposite - положить деньги на счёт\n/withdraw - снять деньги со счёта","Ок","Ок");
			return 1;
		}
		case BANKS_WITHDRAW_CMD:
		{
			new bname[24],
				Float:bcoords[3];
			if(GetBankInfo(PlayerBank[playerid],bname,bcoords[0],bcoords[1],bcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,bcoords[0],bcoords[1],bcoords[2])) goto no_in_range;
				// get count
				itmp[0] = strval(strtok(params,idx));
				if(strlen(params) == 0) itmp[0] = 0;
				// check bug
				if(itmp[0] < 0) itmp[0] *= -1;
				// check
				if(itmp[0] > GetPVarInt(playerid,"BankMoney"))
				{
					format(stmp,sizeof(stmp),"Вы не имеете такой суммы на счету. У вас на счету %d$",GetPVarInt(playerid,"BankMoney"));
					SendClientMessage(playerid,SYSTEM_COLOR,stmp);
					return 1;
				}
				// give
				oGivePlayerMoney(playerid,itmp[0]);
				SetPVarInt(playerid,"BankMoney",(GetPVarInt(playerid,"BankMoney") - itmp[0]));
				// result
				format(stmp,sizeof(stmp),"Вы сняли %d$. У вас на счету %d$",itmp[0],GetPVarInt(playerid,"BankMoney"));
				SendClientMessage(playerid,SYSTEM_COLOR,stmp);
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банке");
			return 1;
		}
		case BANKS_DEPOSITE_CMD:
		{
			new bname[24],
				Float:bcoords[3];
			if(GetBankInfo(PlayerBank[playerid],bname,bcoords[0],bcoords[1],bcoords[2]))
			{
				if(!IsPlayerInRangeOfPoint(playerid,2.5,bcoords[0],bcoords[1],bcoords[2])) goto no_in_range;
				// get count
				itmp[0] = strval(strtok(params,idx));
				if(strlen(params) == 0) itmp[0] = 0;
				// check bug
				if(itmp[0] < 0) itmp[0] *= -1;
				// check
				if(itmp[0] > oGetPlayerMoney(playerid))
				{
					SendClientMessage(playerid,SYSTEM_COLOR,"Вы не имеете такой суммы на руках");
					return 1;
				}
				// give
				oGivePlayerMoney(playerid,-itmp[0]);
				SetPVarInt(playerid,"BankMoney",(GetPVarInt(playerid,"BankMoney") + itmp[0]));
				// result
				format(stmp,sizeof(stmp),"Вы положили %d$. У вас на счету %d$",itmp[0],GetPVarInt(playerid,"BankMoney"));
				SendClientMessage(playerid,SYSTEM_COLOR,stmp);
				return 1;
			}
			no_in_range:
			SendClientMessage(playerid,SYSTEM_COLOR,"Вы не в банке");
			return 1;
		}
	}
	return 0;
}

Banks_OnSavePlayerData(playerid,File:datafile,reason)
{
	#pragma unused reason
	format(stmp,sizeof(stmp),"BankMoney=%d\r\n",GetPVarInt(playerid,"BankMoney"));
	fwrite(datafile,stmp);
}

Banks_OnLoadPlayerData(playerid,datastr[],separatorpos)
{
	if(!strcmp(datastr,"BankMoney",true,separatorpos)) 
	{
		SetPVarInt(playerid,"BankMoney",strval(datastr[separatorpos + 1]));
	}
}
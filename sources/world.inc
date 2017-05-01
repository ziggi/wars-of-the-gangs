/*
*	Created:			04.06.10
*	Author:				009
*	Description:		обьекты, транспорт, классы.
*/

#if defined _world_included
	#endinput
#endif

#define _world_included
#pragma library world

// --------------------------------------------------
// defines
// --------------------------------------------------
#define WORLD_DIR				"world/"
#define OBJECTS_DIR				"objects/"
#define VEHICLES_DIR			"vehicles/"
#define TELEPORTS_FILE			"teleports.txt"
#define OBJECTS_FILE			"objects.txt"
#define VEHICLES_FILE			"vehicles.txt"
#define SELECT_CLASS_INTERIOR	0
#define SELECT_CLASS_POS		-1637.898315,-2239.265136,31.476562
#define SELECT_CLASS_ANGLE		94.0
#define SELECT_CLASS_CAMERA		-1640.841674,-2237.676757,33.083084
#define SELECT_CLASS_LOOK		-1640.059448,-2238.169189,32.701473
// check functions
#if !defined StripNL
		#error StripNL not found in script.
#endif
#if !defined strtok
	#error strtok not found in script.
#endif
#if !defined IsValidObjectModel
	#error IsValidObjectModel not found in script.
#endif
#if !defined IsValidVehicleModel
	#error IsValidVehicleModel not found in script.
#endif
#if !defined IsValidSkin
	#error IsValidSkin not found in script.
#endif

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	WORLD_TELEPORT_CMD
};

// --------------------------------------------------
// statics
// --------------------------------------------------

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock LoadObjectsFromFile(filename[])
{
	new File:df = fopen(filename,io_read),
		k,
		c,
		string[128];
	if(df)
	{
		while(fread(df,string))
		{
			if(!CheckFileLine(stmp)) continue;
			// null and strip
			k = 0;
			// model
			itmp[0] = strval(strtok(string,k,','));
			// coords
			ftmp[0] = floatstr(strtok(string,k,','));
			ftmp[1] = floatstr(strtok(string,k,','));
			ftmp[2] = floatstr(strtok(string,k,','));
			// angles
			ftmp[3] = floatstr(strtok(string,k,','));
			ftmp[4] = floatstr(strtok(string,k,','));
			ftmp[5] = floatstr(strtok(string,k,','));
			// check model
			if(!IsValidObjectModel(itmp[0])) continue;
			// create object
#if defined _streamer_objects_included
			CreateStreamedObject(itmp[0],ftmp[0],ftmp[1],ftmp[2],ftmp[3],ftmp[4],ftmp[5]);
#else
			if(c < MAX_OBJECTS) CreateObject(itmp[0],ftmp[0],ftmp[1],ftmp[2],ftmp[3],ftmp[4],ftmp[5]);
#endif
			// counter
			c++;
		}
		fclose(df);
	}
	return c;
}
stock LoadVehiclesFromFile(filename[])
{
	new File:df = fopen(filename,io_read),
		k,
		c,
		string[128];
	if(df)
	{
		while(fread(df,string))
		{
			
			if(!CheckFileLine(stmp)) continue;
			// null
			k = 0;
			// model
			itmp[0] = strval(strtok(string,k,','));
			// coords
			ftmp[0] = floatstr(strtok(string,k,','));
			ftmp[1] = floatstr(strtok(string,k,','));
			ftmp[2] = floatstr(strtok(string,k,','));
			// angle
			ftmp[3] = floatstr(strtok(string,k,','));
			// check model
			if(!IsValidVehicleModel(itmp[0])) continue;
			// create vehicle
			AddStaticVehicle(itmp[0],ftmp[0],ftmp[1],ftmp[2],ftmp[3],-1,-1);
			// counter
			c++;
		}
		fclose(df);
	}
	return c;
}

stock TeleportTo(playerid,pointid)
{
	new File:df = fopen(MODE_DIR WORLD_DIR TELEPORTS_FILE,io_read),
		c;
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			
			if(c == pointid)
			{
				c = 0;
				// coords
				ftmp[0] = floatstr(strtok(stmp,c,','));
				ftmp[1] = floatstr(strtok(stmp,c,','));
				ftmp[2] = floatstr(strtok(stmp,c,','));
				ftmp[3] = floatstr(strtok(stmp,c,'|'));
				// teleport	
				if(IsPlayerInAnyVehicle(playerid))
				{
					itmp[0] = GetPlayerVehicleID(playerid);
					SetVehiclePos(itmp[0],ftmp[0],ftmp[1],ftmp[2]);
					SetVehicleZAngle(itmp[0],ftmp[3]);
					PutPlayerInVehicle(playerid,itmp[0],0);
				}
				else
				{
					SetPlayerPos(playerid,ftmp[0],ftmp[1],ftmp[2]);
					SetPlayerFacingAngle(playerid,ftmp[3]);
				}
				break;
			}
			else c++;
		}
		fclose(df);
	}
	else print("[ERROR] teleports file not exist");
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
World_OnGameModeInit()
{
	static 
		i,
		counter[3];
	// add all worked classes
	for(i = 0;i < 300;i++)
	{
		if(IsValidSkin(i)) 
		{
			AddPlayerClass(i,0.0,0.0,0.0,0.0,0,0,0,0,0,0);
			// counter
			counter[0]++;
		}
	}
	// load objects from file
	new File:df = fopen(MODE_DIR WORLD_DIR OBJECTS_FILE,io_read);
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// load file
			format(stmp,sizeof(stmp),MODE_DIR WORLD_DIR OBJECTS_DIR "%s",stmp);
			if(fexist(stmp)) counter[1] += LoadObjectsFromFile(stmp);
		}
		fclose(df);
	}
	// load vehicles from file
	df = fopen(MODE_DIR WORLD_DIR VEHICLES_FILE,io_read);
	if(df)
	{
		while(fread(df,stmp))
		{
			if(!CheckFileLine(stmp)) continue;
			// load file
			format(stmp,sizeof(stmp),MODE_DIR WORLD_DIR VEHICLES_DIR "%s",stmp);
			if(fexist(stmp)) counter[2] += LoadVehiclesFromFile(stmp);
		}
		fclose(df);
	}
	// data
#if defined PRINT_STATS_DATA
	printf("Classes: %d",counter[0]);
	printf("Objects: %d",counter[1]);
	printf("Vehicles: %d",counter[2]);
	#if defined _streamer_objects_included
		print("Objects streamer used");
	#else
		print("Objects streamer not used");
	#endif
#endif
	print("World loaded.");
}

World_OnPlayerRequestClass(playerid,classid)
{
	#pragma unused classid
	SetPlayerInterior(playerid,SELECT_CLASS_INTERIOR);
	SetPlayerPos(playerid,SELECT_CLASS_POS);
	SetPlayerFacingAngle(playerid,SELECT_CLASS_ANGLE);
	SetPlayerCameraPos(playerid,SELECT_CLASS_CAMERA);
	SetPlayerCameraLookAt(playerid,SELECT_CLASS_LOOK);	
}

World_Command(playerid,cmdid,params[])
{
	#pragma unused params
	switch(cmdid)
	{
		case WORLD_TELEPORT_CMD:
		{
			// check quest
			if(GetPlayerQuest(playerid) != INVALID_QUEST_ID) return SendClientMessage(playerid,SYSTEM_COLOR,"Вы выполняете задание");
			// file read
			new File:datafile = fopen(MODE_DIR WORLD_DIR TELEPORTS_FILE,io_read),
				line[MAX_STRING];
			// checker
			if(strlen(params) > 0) itmp[0] = 1;
			else itmp[0] = 0;
			itmp[1] = 0;
			if(datafile)
			{
				stmp[0] = 0;
				while(fread(datafile,line))
				{
					if(!CheckFileLine(line)) continue;
					itmp[2] = strfind(line,"|") + 1;
					if(itmp[0])
					{
						if(!strcmp(params,line[ itmp[2] ],true))
						{
							TeleportTo(playerid,itmp[1]);
							fclose(datafile);
							return 1;
						}
						itmp[1]++;
					}

					format(stmp,sizeof(stmp),"%s%s\n",stmp,line[ itmp[2] ]);
				}
				fclose(datafile);
				ShowPlayerDialog(playerid,DIALOG_TELEPORTS,DIALOG_STYLE_LIST,"Телепортация",stmp,"Выбрать","Отмена");
			}
			else print("[ERROR] teleports file not exist");
			return 1;
		}
	}
	return 0;
}

World_Dialog(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case DIALOG_TELEPORTS:
		{
			if(!response) return 1;
			
			TeleportTo(playerid,listitem);
			return 1;
		}
	}
	return 1;
}
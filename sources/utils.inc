/*
*	Created:			15.01.10
*	Author:				009
*	Description:		Мусорка мини функций
*/

#if defined _utils_included
#endinput
#endif

#define _utils_included
#pragma library utils

// --------------------------------------------------
// defines
// --------------------------------------------------
#define MAX_FORMATED_STRING 128
#define MAX_STRTOK_STRING	48

#define NullStr(%1) strmid(%1,"",0,0,strlen(%1))
#define SendFormatedMessage(%1,%2,%3) format(FormatedMsgString,MAX_FORMATED_STRING,%3); \
		SendClientMessage(%1,%2,FormatedMsgString)
#define WriteFileData(%1,%2,%3) format(%2,sizeof(%2),%3); \
		fwrite(%1,%2); \
		fwrite(%1,"\r\n")
#define ReadFileDataStr(%1,%2,%3,%4) if(!strcmp(%1,%2,true,%3)) strmid(%4,%1,(%3 + 1),strlen(%1))
#define ReadFileDataInt(%1,%2,%3,%4) if(!strcmp(%1,%2,true,%3)) %4 = strval(%1[%3 + 1])
#define ReadFileDataFloat(%1,%2,%3,%4) if(!strcmp(%1,%2,true,%3)) %4 = floatstr(%1[%3 + 1])
#define GetDistanceBetweenCoords(%1,%2,%3,%4,%5,%6) floatsqroot((%4 - %1)*(%4 - %1) + (%5 - %2)*(%5 - %2) + (%6 - %3)*(%6 - %3))
// by Y_Less
#if cellbits == 32
	#define CELLSHIFT (5)
#else
	#if cellbits == 64
		#define CELLSHIFT (6)
	#else
		#if cellbits == 16
			#define CELLSHIFT (4)
		#else
			#error Unkown cell size
		#endif
	#endif
#endif

// --------------------------------------------------
// news
// --------------------------------------------------
stock FormatedMsgString[MAX_FORMATED_STRING];
stock PlayerVehicle[][] = {
"Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel", "Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana",
"Infernus", "Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam", "Esperanto", "Taxi", "Washington", "Bobcat",
"Whoopee", "BF Injection", "Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus", "Rhino", "Barracks", "Hotknife",
"Trailer 1", "Previon", "Coach", "Cabbie", "Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral", "Squalo",
"Seasparrow", "Pizzaboy", "Tram", "Trailer 2", "Turismo", "Speeder", "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair",
"Berkley's RC Van", "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale", "Oceanic", "Sanchez", "Sparrow",
"Patriot", "Quad", "Coastguard", "Dinghy", "Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX", "Burrito",
"Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper", "Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring",
"Sandking", "Blista Compact", "Police Maverick", "Boxvillde", "Benson", "Mesa", "RC Goblin", "Hotring Racer A", "Hotring Racer B",
"Bloodring Banger", "Rancher", "Super GT", "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster","Stunt",  "Tanker",
"Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra", "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer", "Remington", "Slamvan", "Blade", "Freight", "Streak",
"Vortex", "Vincent", "Bullet", "Clover", "Sadler", "Firetruck LA", "Hustler", "Intruder", "Primo", "Cargobob", "Tampa", "Sunrise", "Merit",
"Utility", "Nevada", "Yosemite", "Windsor", "Monster A", "Monster B", "Uranus", "Jester", "Sultan", "Stratum", "Elegy", "Raindance",
"RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito", "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper", "Broadway",
"Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400", "News Van", "Tug", "Trailer 3", "Emperor", "Wayfarer", "Euros", "Hotdog",
"Club", "Freight Carriage", "Trailer 4", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car (LSPD)", "Police Car (SFPD)",
"Police Car (LVPD)", "Police Ranger", "Picador", "S.W.A.T", "Alpha", "Phoenix", "Glendale", "Sadler", "Luggage Trailer A",
"Luggage Trailer B", "Stairs", "Boxville", "Tiller", "Utility Trailer" };
stock strtoktmp[MAX_STRTOK_STRING];
static OffRoad[] =
{
	400,
	424,
	444,
	470,
	489,
	495,
	500,
	505,
	556,
	557,
	568,
	573,
	579
};
static Saloons[] =
{
	401,
	410,
	419,
	421,
	426,
	436,
	445,
	466,
	467,
	474,
	491,
	492,
	504,
	507,
	516,
	517,
	518,
	526,
	527,
	529,
	540,
	542,
	546,
	547,
	549,
	550,
	551,
	560,
	580,
	585,
	604
};
static Sport[] =
{
	402,
	411,
	415,
	429,
	451,
	475,
	477,
	494,
	496,
	502,
	503,
	506,
	541,
	558,
	559,
	565,
	587,
	589,
	602,
	603
};
static Industrial[] =
{
	403,
	408,
	413,
	414,
	422,
	440,
	443,
	455,
	456,
	459,
	478,
	482,
	498,
	499,
	514,
	515,
	524,
	531,
	543,
	552,
	554,
	578,
	582,
	600,
	605,
	609
};
static StationWagons[] =
{
	404,
	418,
	458,
	479,
	561
};
static Lowriders[] =
{
	412,
	534,
	535,
	536,
	566,
	567,
	575,
	576
};
static Convertibles[] =
{
	439,
	480,
	533,
	555
};
static PublicService[] =
{
	407,
	416,
	420,
	427,
	431,
	432,
	433,
	437,
	438,
	490,
	528,
	544,
	596,
	597,
	598,
	599,
	601
};
static Helicopters[] =
{
	417,
	425,
	447,
	469,
	487,
	488,
	497,
	548,
	563
};
static Airplane[] =
{
	460,
	476,
	511,
	512,
	513,
	519,
	520,
	553,
	577,
	592,
	593
};
static Boats[] =
{
	430,
	446,
	452,
	453,
	454,
	472,
	473,
	484,
	493,
	595
};
static Trailers[] =
{
	435,
	450,
	569,
	570,
	584,
	606,
	607,
	608,
	610,
	611
};
static Bikes[] =
{
	448,
	461,
	462,
	463,
	468,
	471,
	481,
	509,
	510,
	521,
	522,
	523,
	581,
	586
};
static Unique[] =
{
	406,
	409,
	423,
	428,
	434,
	442,
	449,
	457,
	483,
	485,
	486,
	508,
	525,
	530,
	532,
	537,
	538,
	539,
	545,
	571,
	572,
	574,
	583,
	588,
	590,
	591
};

// --------------------------------------------------
// enums
// --------------------------------------------------
enum
{
	VEHICLE_DOOR_UNDAMAGED, // цело
	VEHICLE_DOOR_SWINGING, // открыто(качается)
	VEHICLE_DOOR_DAMAGED, // помято
	VEHICLE_DOOR_DAMAGED_SWINGING, // помято и открыто(качается)
	VEHICLE_DOOR_FELL_OF // оторвано
};

enum
{
	VEHICLE_BOOT_UNDAMAGED, // цело
	VEHICLE_BOOT_SWINGING, // открыто(качается)
	VEHICLE_BOOT_DAMAGED, // помято
	VEHICLE_BOOT_DAMAGED_SWINGING, // помято и открыто(качается)
	VEHICLE_BOOT_FELL_OF // оторвано
};

enum
{
	VEHICLE_BONNET_UNDAMAGED, // цело
	VEHICLE_BONNET_SWINGING, // открыто(качается)
	VEHICLE_BONNET_DAMAGED, // помято
	VEHICLE_BONNET_DAMAGED_SWINGING, // помято и открыто(качается)
	VEHICLE_BONNET_FELL_OF // оторвано
};

enum
{
	VEHICLE_WINDSHIELD_UNDAMAGED, // цело
	VEHICLE_WINDSHIELD_CRACKED_1, // треснуло
	VEHICLE_WINDSHIELD_CRACKED_2, // треснуло
	VEHICLE_WINDSHIELD_DESTROYED // разбито
};

enum
{
	VEHICLE_TIRE_UNDAMAGED, // цело
	VEHICLE_TIRE_DAMAGED // пробито
};

enum
{
	VEHICLE_LIGHT_UNDAMAGED, // целы
	VEHICLE_LIGHT_DAMAGED // разбиты
};

// --------------------------------------------------
// natives list (for pawno functions list)
// --------------------------------------------------
/*
native oGetPlayerName(playerid);
native GetXYInFrontOfPlayer(playerid,&Float:X,&Float:Y,Float:distance);
native StripNL(str[]);
native Float:oGetPlayerHealth(playerid);
native strtok(const string[],&index,seperator=' ');
native SendFormatedMessage(playerid,color,const format[],{Float,_}:...);
native IsPlayerInRangeOfPoint(playerid, Float:range, Float:x, Float:y, Float:z);
native WriteFileData(File:file,temp[],const format[],{Float,_}:...);
native ReadFileDataStr(const source[],const key[],seperator_pos,dest[]);
native ReadFileDataInt(const source[],const key[],seperator_pos,&dest);
native ReadFileDataFloat(const source[],const key[],seperator_pos,&Float:dest);
native GetDistanceBetweenCoords(Float:X1,Float:Y1,Float:Z1,Float:X2,Float:Y2,Float:Z2);
native IsValidObjectModel(modelid);
native IsValidVehicleModel(modelid);
native IsValidSkin(skinid);
*/

// --------------------------------------------------
// stocks
// --------------------------------------------------
stock oGetPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof (name));
	return name;
}

stock oGetWeaponName(weaponid)
{
	new name[MAX_PLAYER_NAME];
	if(weaponid != 50) GetWeaponName(weaponid, name, sizeof (name));
	else name = "Dinamite";
	return name;
}

stock oGetVehicleName(vmodelid)
{
	new name[MAX_PLAYER_NAME];
	format(name,sizeof(name),PlayerVehicle[vmodelid - 400]);
	return name;
}

stock GetXYInFrontOfPlayer(playerid,&Float:X,&Float:Y,Float:distance)
{
	new Float:A;
	GetPlayerPos(playerid,X,Y,A);
    GetPlayerFacingAngle(playerid,A);
	X += (distance * floatsin(-A, degrees));
	Y += (distance * floatcos(-A, degrees));
}

stock StripNL(str[])
{
	new
		l = strlen(str);
	while (l-- && str[l] <= ' ') str[l] = '\0';
}

stock CheckFileLine(line[],size=sizeof(line))
{
	static idx;
	// check invalid letter
	if(line[0] == 0xFEFF) format(line,size,line[1]);
	// check unused str
	if(line[0] == '~') return 0;
	// delete comment
	if((idx = strfind(line,"//")) != -1) strdel(line,idx,strlen(line));
	// strip
	StripNL(line);
	return 1;
}

stock strtok(const string[],&index,seperator=' ')
{
	new offset = index,
		res[MAX_STRTOK_STRING],
		length = strlen(string);
	while((index < length) && (string[index] != seperator))
	{
		res[index - offset] = string[index];
		index++;
	}
	res[index - offset] = EOS;
	index++;
	return res;
}

stock GetWeaponSlot(weaponid)
{
	switch(weaponid)
	{
		case 0,1: 				return 0;
		case 2,3,4,5,6,7,8,9:	return 1;
		case 22,23,24:			return 2;
		case 25,26,27:			return 3;
		case 28,29,32:			return 4;
		case 30,31:				return 5;
		case 33,34:				return 6;
		case 35,36,37,38:		return 7;
		case 16,17,18,39:		return 8;
		case 41,42,43:			return 9;
		case 10,11,12,13,14,15: return 10;
		case 44,45,46:			return 11;
		case 40:				return 12;
	}
	return 0;
}

stock encode_panels(flp, frp, rlp, rrp, windshield, front_bumper, rear_bumper)
{
	return flp | (frp << 4) | (rlp << 8) | (rrp << 12) | (windshield << 16) | (front_bumper << 20) | (rear_bumper << 24);
}

stock encode_doors(bonnet, boot, driver_door, passenger_door)
{
	return bonnet | (boot << 8) | (driver_door << 16) | (passenger_door << 24);
}

stock encode_lights(light1, light2, light3, light4)
{
	return light1 | (light2 << 1) | (light3 << 2) | (light4 << 3);
}

stock encode_tires(tire1, tire2, tire3, tire4)
{
	return tire1 | (tire2 << 1) | (tire3 << 2) | (tire4 << 3);
}
// специально для байков
stock encode_tires_bike(rear, front)
{
	return rear | (front << 1);
} 

stock IsPosInRangeOfPoint(Float:px,Float:py,Float:pz,Float:range,Float:x,Float:y,Float:z)
{
	x -= px;
	y -= py;
	z -= pz;
	return ((x * x) + (y * y) + (z * z) < (range * range));
}

stock IsValidObjectModel(objmodelid) // thanks for Y_Less
{
	static
		modeldat[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -128,
			-515899393, -134217729, -1, -1, 33554431, -1, -1, -1, -14337, -1, -33,
			127, 0, 0, 0, 0, 0, -8388608, -1, -1, -1, -16385, -1, -1, -1, -1, -1,
			-1, -1, -33, -1, -771751937, -1, -9, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, 33554431, -25, -1, -1, -1, -1, -1, -1,
			-1073676289, -2147483648, 34079999, 2113536, -4825600, -5, -1, -3145729,
			-1, -16777217, -63, -1, -1, -1, -1, -201326593, -1, -1, -1, -1, -1,
			-257, -1, 1073741823, -133122, -1, -1, -65, -1, -1, -1, -1, -1, -1,
			-2146435073, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1073741823, -64, -1,
			-1, -1, -1, -2635777, 134086663, 0, -64, -1, -1, -1, -1, -1, -1, -1,
			-536870927, -131069, -1, -1, -1, -1, -1, -1, -1, -1, -16384, -1,
			-33554433, -1, -1, -1, -1, -1, -1610612737, 524285, -128, -1,
			2080309247, -1, -1, -1114113, -1, -1, -1, 66977343, -524288, -1, -1, -1,
			-1, -2031617, -1, 114687, -256, -1, -4097, -1, -4097, -1, -1,
			1010827263, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -32768, -1, -1, -1, -1, -1,
			2147483647, -33554434, -1, -1, -49153, -1148191169, 2147483647,
			-100781080, -262145, -57, 134217727, -8388608, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1048577, -1, -449, -1017, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1835009, -2049, -1, -1, -1, -1, -1, -1,
			-8193, -1, -536870913, -1, -1, -1, -1, -1, -87041, -1, -1, -1, -1, -1,
			-1, -209860, -1023, -8388609, -2096897, -1, -1048577, -1, -1, -1, -1,
			-1, -1, -897, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1610612737,
			-3073, -28673, -1, -1, -1, -1537, -1, -1, -13, -1, -1, -1, -1, -1985,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1056964609, -1, -1, -1,
			-1, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-236716037, -1, -1, -1, -1, -1, -1, -1, -536870913, 3, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -2097153, -2109441, -1, 201326591, -4194304, -1, -1,
			-241, -1, -1, -1, -1, -1, -1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -32768, -1, -1, -1, -2, -671096835, -1, -8388609, -66323585, -13,
			-1793, -32257, -247809, -1, -1, -513, 16252911, 0, 0, 0, -131072,
			33554383, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8356095, 0, 0, 0, 0, 0,
			0, -256, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-268435449, -1, -1, -2049, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			92274627, -65536, -2097153, -268435457, 591191935, 1, 0, -16777216, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 127
		};
	return (modeldat[(objmodelid) >>> CELLSHIFT]) & (1 << ((objmodelid) & (cellbits - 1)));
}

stock IsValidVehicleModel(vehmodelid)
{
	if(vehmodelid < 200) return 0;
	if(vehmodelid > 611) return 0;
	return 1;
}


stock IsValidSkin(skinid)
{
	static DisableSkins[] = {3, 4, 5, 6, 8, 42, 65, 74, 86, 119, 149, 208, 273, 289};
	for(new k = 0;k < sizeof(DisableSkins);k++)
	{
		if(DisableSkins[k] == skinid) return 0;
	}
	return 1;
}

stock SetPlayerColorAlpha(playerid, alpha) //By Betamaster
{
	new r, g, b, a;
	HexToRGBA(GetPlayerColor(playerid), r, g, b, a);
	SetPlayerColor(playerid, RGBAToHex(r, g, b, alpha));
}

stock RGBAToHex(r, g, b, a) //By Betamaster
{
	return (r<<24 | g<<16 | b<<8 | a);
}

stock HexToRGBA(colour, &r, &g, &b, &a) //By Betamaster
{
	r = (colour >> 24) & 0xFF;
	g = (colour >> 16) & 0xFF;
	b = (colour >> 8) & 0xFF;
	a = colour & 0xFF;
}

stock memset(data[],value,size = sizeof(data))
{
	for(new i = 0;i < size;i++) data[i] = value;
}

stock mktime(hour,minute,second,day,month,year) //By mabako
{
	new timestamp;
	timestamp = second;
	timestamp += minute * 60;
	timestamp += hour * 3600;
	new days_of_month[12];
	if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) days_of_month = {31,29,31,30,31,30,31,31,30,31,30,31};
	else days_of_month = {31,28,31,30,31,30,31,31,30,31,30,31};
	new days_this_year = 0;
	days_this_year = day;
	if(month > 1) 
	{
		for(new i=0; i<month-1;i++) days_this_year += days_of_month[i];
	}
	timestamp += days_this_year * 86400;
	for(new j = 1970;j < year;j++) 
	{
		timestamp += 31536000;
		if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) timestamp += 86400;
	}
	return timestamp;
}

stock GetVehicleModelByType(type)
{
	switch(type)
	{
		case 0: return OffRoad[random(sizeof(OffRoad))];
		case 1: return Saloons[random(sizeof(Saloons))];
		case 2: return Sport[random(sizeof(Sport))];
		case 3: return Industrial[random(sizeof(Industrial))];
		case 4: return StationWagons[random(sizeof(StationWagons))];
		case 5: return Lowriders[random(sizeof(Lowriders))];
		case 6: return Convertibles[random(sizeof(Convertibles))];
		case 7: return PublicService[random(sizeof(PublicService))];
		case 8: return Helicopters[random(sizeof(Helicopters))];
		case 9: return Airplane[random(sizeof(Airplane))];
		case 10: return Boats[random(sizeof(Boats))];
		case 11: return Trailers[random(sizeof(Trailers))];
		case 12: return Bikes[random(sizeof(Bikes))];
		case 13: return Unique[random(sizeof(Unique))];
	}
	return 400;
}
#ifndef __OGP_STRUCTS_H__
#define __OGP_STRUCTS_H__

#include "ogp_settings.h"
#include "ogp_basictypes.h"

typedef enum {
	OGP_Error_None				= -2,
	OGP_Error_DontSend			= -1,
	OGP_Error_Banned			= 0,
	OGP_Error_InvalidType		= 1,
	OGP_Error_InvalidValue		= 2,
	OGP_Error_InvalidChallenge	= 3,
	OGP_Error_InvalidQuery		= 4,
} EOGP_Error;

typedef enum {
	OGP_QueryType_Ping			= 0,
	OGP_QueryType_Default1		= 1,
	OGP_QueryType_Error			= 0xFF,
} EOGP_QueryType;

typedef enum {
	OGP_Banned_Not				= 0,
	OGP_Banned_SendInfo			= 1,
	OGP_Banned_Ignore			= 2,
} EOGP_Banned;


#define		OGP_SERVERFLAGS_TYPE_UNKOWN			0
#define		OGP_SERVERFLAGS_TYPE_LISTEN			1
#define		OGP_SERVERFLAGS_TYPE_DEDICATED		2

#define		OGP_SERVERFLAGS_OS_UNKOWN			0
#define		OGP_SERVERFLAGS_OS_WINDOWS			1
#define		OGP_SERVERFLAGS_OS_LINUX			2
#define		OGP_SERVERFLAGS_OS_MAC				3

#define		OGP_LIMITTYPE_TIME					0
#define		OGP_LIMITTYPE_PLAYERSCORE			1
#define		OGP_LIMITTYPE_ROUND					2
#define		OGP_LIMITTYPE_TEAMSCORE				3

typedef struct {
	ogp_uint8_t bServerInfo : 1;
	ogp_uint8_t bTeamList : 1;
	ogp_uint8_t bPlayerList : 1;
	ogp_uint8_t bRuleList : 1;
	ogp_uint8_t bAddOnList : 1;
	ogp_uint8_t bLimitList : 1;
	ogp_uint8_t Reserved2 : 2;

	ogp_uint8_t bColoredNames : 1;
	ogp_uint8_t Reserved1 : 7;
} ogp_request_flags_t;

typedef struct {
	ogp_uint8_t bGameName : 1;
	ogp_uint8_t bServerFlags : 1;
	ogp_uint8_t bHostName : 1;
	ogp_uint8_t bConnectPort : 1;
	ogp_uint8_t Reserved3 : 4;

	ogp_uint8_t bMod : 1;
	ogp_uint8_t bGameType : 1;
	ogp_uint8_t bGameMode : 1;
	ogp_uint8_t bMap : 1;
	ogp_uint8_t bNextMap : 1;
	ogp_uint8_t Reserved4 : 3;

	ogp_uint8_t bPlayerCount : 1;
	ogp_uint8_t bSlotMax : 1;
	ogp_uint8_t bBotCount : 1;
	ogp_uint8_t bReservedSlots : 1;
	ogp_uint8_t Reserved5 : 4;
} ogp_serverinfo_fields_t;

typedef struct {
	ogp_uint8_t Type : 2;				// 0 - Unkown, 1 - Listen, 2 - Dedicated
	ogp_uint8_t bPassword : 1;
	ogp_uint8_t bProxy : 1;
	ogp_uint8_t OperatingSystem : 3;	// 0 - Unkown, 1 - Windows, 2 - Linux, 3 - Mac
	ogp_uint8_t Reserved : 1;
} ogp_serverinfo_serverflags_t;

typedef struct {
	ogp_uint8_t bModIdentifier : 1;
	ogp_uint8_t bModSize : 1;
	ogp_uint8_t bModVersion : 1;
	ogp_uint8_t bModURL : 1;
	ogp_uint8_t bModAuthor : 1;
	ogp_uint8_t Reserved : 3;
} ogp_mod_fields_t;

typedef struct {
	ogp_uint8_t bMapFileName : 1;
	ogp_uint8_t bMapFileSize : 1;
	ogp_uint8_t bMapFileMD5 : 1;
	ogp_uint8_t bMapVersion : 1;
	ogp_uint8_t bMapURL : 1;
	ogp_uint8_t bMapAuthor : 1;
	ogp_uint8_t Reserved : 2;
} ogp_map_fields_t;

typedef struct {
	ogp_uint8_t bTeamName : 1;
	ogp_uint8_t bTeamScore : 1;
	ogp_uint8_t bTeamAveragePing : 1;
	ogp_uint8_t bTeamAverageLoss : 1;
	ogp_uint8_t bTeamPlayerCount : 1;
	ogp_uint8_t bTeamColor : 1;
	ogp_uint8_t Reserved : 2;
} ogp_teamlist_fields_t;

typedef struct {
	ogp_uint8_t bPlayerFlags : 1;
	ogp_uint8_t bPlayerSlot : 1;
	ogp_uint8_t bPlayerName : 1;
	ogp_uint8_t bPlayerTeam : 1;
	ogp_uint8_t bPlayerClass : 1;
	ogp_uint8_t bPlayerRace : 1;
	ogp_uint8_t Reserved1 : 2;

	ogp_uint8_t bPlayerScore : 1;
	ogp_uint8_t bPlayerFrags : 1;
	ogp_uint8_t bPlayerKills : 1;
	ogp_uint8_t bPlayerDeath : 1;
	ogp_uint8_t bPlayerSuicides : 1;
	ogp_uint8_t bPlayerTeamKills : 1;
	ogp_uint8_t Reserved2 : 2;

	ogp_uint8_t bPlayerID : 1;
	ogp_uint8_t bPlayerGlobalID : 1;
	ogp_uint8_t bPlayerPing : 1;
	ogp_uint8_t bPlayerLoss : 1;
	ogp_uint8_t bPlayerModel : 1;
	ogp_uint8_t bPlayerTime : 1;
	ogp_uint8_t Reserved3 : 2;

	ogp_uint8_t bPlayerAddress : 1;
	ogp_uint8_t Reserved4 : 7;
} ogp_playerlist_fields_t;

typedef struct {
	ogp_uint8_t	bAlive : 1;
	ogp_uint8_t	bDead : 1;
	ogp_uint8_t	bBot : 1;
	ogp_uint8_t	Reserved : 5;
} ogp_playerlist_flags_t;

typedef struct {
	ogp_uint8_t bAddOnFlags : 1;
	ogp_uint8_t bAddOnShortName : 1;
	ogp_uint8_t bAddOnLongName : 1;
	ogp_uint8_t bAddOnVersion : 1;
	ogp_uint8_t Reserved : 4;
} ogp_addonlist_fields_t;

typedef struct {
	ogp_uint8_t bActive : 1;
	ogp_uint8_t bAntiCheatTool : 1;
	ogp_uint8_t bMutator : 1;
	ogp_uint8_t bAdminTool : 1;
} ogp_addonlist_flags_t;

typedef struct {
	ogp_uint8_t bLimitLeft : 1;
	ogp_uint8_t Type : 4;
	ogp_uint8_t Reserved : 3;
} ogp_limitlist_type_t;

typedef struct ogp_serverdata_t {
	ogp_uint16_t		GameID;
	ogp_request_flags_t	RequestFlags;

	struct server_info_t {
		ogp_serverinfo_fields_t	Fields;

		OGP_CHAR		GameName[32];

		ogp_serverinfo_serverflags_t	ServerFlags;

		OGP_CHAR		HostName[256];
		//! Color is not implemented completely atm
		ogp_uint8_t		HostNameColorInfo[256];
		ogp_uint32_t	HostNameColorInfoLen;

		ogp_uint16_t	ConnectPort;


		struct mod_t {
			ogp_mod_fields_t	Fields;

			OGP_CHAR		ModName[32];
			OGP_CHAR		ModIdentifier[32];
			ogp_uint32_t	ModSize;
			OGP_CHAR		ModVersion[32];
			OGP_CHAR		ModURL[256];
			OGP_CHAR		ModAuthor[32];
		} Mod;

		OGP_CHAR		GameType[64];
		OGP_CHAR		GameMode[64];

		ogp_map_fields_t	MapFields;
		struct map_t {
			OGP_CHAR		MapName[32];
			OGP_CHAR		MapFileName[32];
			ogp_uint32_t	MapFileSize;
			ogp_uint8_t		MapFileMD5[16];
			OGP_CHAR		MapVersion[32];
			OGP_CHAR		MapURL[256];
			OGP_CHAR		MapAuthor[32];
		} Map,NextMap;


		ogp_uint32_t	PlayerCount;
		ogp_uint32_t	SlotMax;
		ogp_uint32_t	BotCount;
		ogp_uint32_t	ReservedSlots;
	} ServerInfo;

	struct team_list_t {
		ogp_uint32_t			Count;
		ogp_teamlist_fields_t	Fields;

		struct team_t {
			OGP_CHAR		Name[64];
			//! Color is not implemented completely atm
			ogp_uint8_t		NameColorInfo[256];
			ogp_uint32_t	NameColorInfoLen;
			ogp_int32_t		Score;
			ogp_uint16_t	AveragePing;
			ogp_uint16_t	AverageLoss;
			ogp_uint32_t	PlayerCount;
			ogp_uint16_t	Color;
		} List[OGP_TEAMLIST_MAX];
	} Team;

	struct player_list_t {
		ogp_uint32_t	Count;
		ogp_playerlist_fields_t	Fields;

		struct player_t {
			ogp_playerlist_flags_t	Flags;

			ogp_uint32_t	Slot;
			OGP_CHAR		Name[64];
			//! Color is not implemented completely atm
			ogp_uint8_t		NameColorInfo[256];
			ogp_uint32_t	NameColorInfoLen;
			ogp_int32_t		TeamNo;
			OGP_CHAR		Class[64];
			OGP_CHAR		Race[64];

			ogp_int32_t		Score;
			ogp_int32_t		Frags;
			ogp_uint32_t	Kills;
			ogp_uint32_t	Death;
			ogp_uint32_t	Suicides;
			ogp_uint32_t	TeamKills;

			ogp_uint32_t	ID;
			OGP_CHAR		GlobalID[64];
			ogp_uint16_t	Ping;
			ogp_uint16_t	Loss;
			OGP_CHAR		Model[64];
			ogp_uint16_t	Time;

			ogp_uint8_t		Addr[64];
			ogp_uint32_t	AddrLen;
		} List[OGP_PLAYERLIST_MAX];
	} Player;

	struct rule_list_t {
		ogp_uint32_t	Count;

		struct rule_t {
			OGP_CHAR	Key[64];
			OGP_CHAR	Value[256];
		} List[OGP_RULELIST_MAX];		
	} Rule;

	struct addon_list_t {
		ogp_uint32_t	Count;
		ogp_addonlist_fields_t	Fields;

		struct addon_t {
			ogp_addonlist_flags_t	Flags;

			OGP_CHAR	ShortName[32];
			OGP_CHAR	LongName[64];
			OGP_CHAR	Version[32];
		} List[OGP_ADDONLIST_MAX];		
	} AddOn;

	struct limit_list_t {
		ogp_uint32_t	Count;
		
		struct limit_t {
			ogp_limitlist_type_t	LimitType;

			ogp_uint32_t	Limit;
			ogp_uint32_t	Left;
		} List[OGP_LIMITLIST_MAX];
	} Limit;
} ogp_serverdata_t;

#endif


#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "ogp_server.h"
#include "ogp_settings.h"
#include "ogp_intern.h"


#define		AF_INET		2
#define		AF_IPX		4
#define		AF_INET6	10


OGP_INLINE ogp_uint8_t OGP_GetUINT8(const ogp_uint8_t** pBuf) {
	return *(*pBuf)++;
}

OGP_INLINE ogp_uint16_t OGP_GetUINT16(const ogp_uint8_t** pBuf) {
	ogp_uint16_t Value = *(const ogp_uint16_t*)*pBuf;
	*pBuf += sizeof(Value);
	return Value;
}

OGP_INLINE ogp_uint32_t OGP_GetUINT32(const ogp_uint8_t** pBuf) {
	ogp_uint32_t Value = *(const ogp_uint32_t*)*pBuf;
	*pBuf += sizeof(Value);
	return Value;
}

static void OGP_GetVarBitArray(const ogp_uint8_t** pBuf, void* pData, ogp_uint32_t DataLen) {
	const ogp_uint8_t* pOldBuf = *pBuf;
	ogp_uint32_t Len;

	while (OGP_GetUINT8(pBuf) & 0x80)
		;

	Len = *pBuf - pOldBuf;
	if (Len > DataLen)
		Len = DataLen;
	memcpy(pData, pOldBuf, Len);
	if (Len < DataLen)
		memset((ogp_uint8_t*)pData+Len, 0, DataLen-Len);
}

static ogp_uint32_t OGP_GetNextRequestID() {
	static ogp_uint32_t RequestID = 0;
	return ++RequestID;
}

static ogp_uint32_t OGP_GetRandomUINT32() {
	union {
		ogp_uint32_t	dw;
		ogp_uint16_t	w[2];
	} u;

	u.w[0] = (ogp_uint16_t)rand();
	srand(u.w[0]+(int)time(NULL));
	u.w[1] = (ogp_uint16_t)rand();

	return u.dw;
}

// The minimum value of pDataLen is 4
static ogp_uint32_t OGP_CalcTempHash(ogp_uint8_t* pData, int DataLen) {
	static time_t LastUpdate = 0;
	static ogp_uint32_t Key1,Key2;
	ogp_uint32_t Hash = 0;
	int i;
	time_t CurrentTime = time(NULL);

	if (LastUpdate == 0 || LastUpdate + OGP_CHALLENGE_TIMEOUT < CurrentTime) {
		LastUpdate = CurrentTime;
		Key1 = OGP_GetRandomUINT32();
		Key2 = OGP_GetRandomUINT32();
	}

	for (i=0; i<DataLen; i+=4) {
		ogp_uint32_t Value = 0;
		union {
			double	ResDouble;
			ogp_uint32_t	ResDW[2];
		} u[2];

		if (i > DataLen-4) {
			memcpy(&Value, &pData[i], DataLen - i);
		} else {
			Value = *(ogp_uint32_t*)&pData[i];
		}

		u[0].ResDouble = pow((double)Value+ 2, ((double)Key1 + 2)/0xFFFFFFFFu);
		u[1].ResDouble = pow((double)Key2 + 2, ((double)Value+ 2)/0xFFFFFFFFu);

		Hash ^= u[0].ResDW[0] ^ u[0].ResDW[1] ^ u[1].ResDW[0] ^ u[1].ResDW[1];
	}

	return Hash;
}

static ogp_uint32_t OGP_GetChallengeNo(void* __SockAddr, int SockAddrLen) {
	ogp_uint8_t* SockAddr = __SockAddr;

	if (*(ogp_uint16_t*)&SockAddr[0] == AF_INET && SockAddrLen >= 8) {
		SockAddr += 4;
		SockAddrLen = 4;
	} else if (*(ogp_uint16_t*)&SockAddr[0] == AF_INET6 && SockAddrLen >= 28) {
		SockAddr += 8;
		SockAddrLen = 16;
	} else if (*(ogp_uint16_t*)&SockAddr[0] == AF_IPX && SockAddrLen >= 15) {
		SockAddr += 8;
		SockAddrLen = 6;
	} else {
		SockAddr += 2;
		SockAddrLen -= 2;
	}

	return OGP_CalcTempHash(SockAddr, SockAddrLen);
}

const ogp_serverdata_t* OGP_GetServerData() {
	static ogp_serverdata_t	s_ServerData;
	static time_t LastUpdate = 0;

	time_t CurrentTime = time(NULL);

	if (LastUpdate != CurrentTime) {
		LastUpdate = CurrentTime;

		memset(&s_ServerData, 0, sizeof(s_ServerData));
		OGP_UpdateServerData(&s_ServerData);
	}

	return &s_ServerData;
}


#if OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_CHAR

static void OGP_SndBufAdd_String(parse_info_t* pPI, const char* SzString) {
	size_t Len = strlen(SzString)+1;

	if (pPI->pSndBufEnd + Len >= pPI->pSndBufMax) {
		pPI->pSndBufEnd = pPI->pSndBufMax;
		return;
	}

	// bUTF8 is deactivated
	OGP_SndBufAdd_Buf(pPI, (const ogp_uint8_t*)SzString, Len);
}

#elif OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_WCHAR

static void OGP_SndBufAdd_String(parse_info_t* pPI, const wchar_t* SzString) {
	if (pPI->Head.Flags.bUTF8) {
		size_t Len = OGP_UnicodeToUTF8_GetLength(SzString)+1;

		if (pPI->pSndBufEnd + Len >= pPI->pSndBufMax) {
			pPI->pSndBufEnd = pPI->pSndBufMax;
			return;
		}

		OGP_strncpy_UnicodeToUTF8(pPI->pSndBufEnd, SzString, Len);

		pPI->pSndBufEnd += Len;
	} else {
		size_t Len = wcslen(SzString)+1;

		if (pPI->pSndBufEnd + Len >= pPI->pSndBufMax) {
			pPI->pSndBufEnd = pPI->pSndBufMax;
			return;
		}

		OGP_strncpy_UnicodeToAscii(pPI->pSndBufEnd, SzString, Len);

		pPI->pSndBufEnd += Len;
	}
}

#elif OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_UTF8

static void OGP_SndBufAdd_String(parse_info_t* pPI, const char* SzString) {
	if (pPI->Head.Flags.bUTF8) {
		size_t Len = strlen(SzString)+1;

		if (pPI->pSndBufEnd + Len >= pPI->pSndBufMax) {
			pPI->pSndBufEnd = pPI->pSndBufMax;
			return;
		}

		// bUTF8 is deactivated
		OGP_SndBufAdd_Buf(pPI, (const ogp_uint8_t*)SzString, Len);
	} else {
		size_t Len = OGP_UTF8_GetLength(SzString)+1;

		if (pPI->pSndBufEnd + Len >= pPI->pSndBufMax) {
			pPI->pSndBufEnd = pPI->pSndBufMax;
			return;
		}

		OGP_strncpy_UTF8ToAscii(pPI->pSndBufEnd, SzString, Len);
		pPI->pSndBufEnd += Len;
	}
}

#else
#	error Invalid OGP_CHAR_ENCODING
#endif


static void OGP_Send(parse_info_t* pPI) {
	ogp_header_flags_t Flags = {0};
	ogp_uint32_t	PacketCount;
	ogp_uint32_t	PacketSize;
	ogp_uint32_t	PacketNo;
	ogp_uint8_t*	pStart = pPI->pSndBufStart;

	if (pPI->Head.MaxBytesPerPacket) {
		ogp_uint32_t	MaxPacketLen = pPI->Head.MaxBytesPerPacket - OGP_MAX_ANSWER_HEADER_LENGTH;
		PacketCount = (pPI->pSndBufEnd - pPI->pSndBufStart) / MaxPacketLen + 1;
		PacketSize = (pPI->pSndBufEnd - pPI->pSndBufStart) / PacketCount;
	} else {
		PacketCount = 1;
		PacketSize = pPI->pSndBufEnd - pPI->pSndBufStart;
	}

	if (PacketCount >= 256)
		return;

	Flags.bAnswer = true;
	Flags.bChallengeNumber = pPI->Head.Flags.bChallengeNumber;
	Flags.bSplit = (PacketCount > 1);
	Flags.bRequestID = (Flags.bSplit || pPI->Head.Flags.bRequestID);
	if (Flags.bRequestID && pPI->Head.RequestID == 0)
		pPI->Head.RequestID = OGP_GetNextRequestID();
	Flags.bUTF8 = pPI->Head.Flags.bUTF8;

	for (PacketNo=0; PacketNo<PacketCount; ++PacketNo, pStart += PacketSize) {
		ogp_uint8_t*	pStart2;
		ogp_uint8_t*	pEnd = (PacketNo == PacketCount-1) ? (pPI->pSndBufEnd) : (pStart + PacketSize);

		if (PacketNo == 1)
			Flags.bChallengeNumber = false;
		
		pStart2 = OGP_CreateHeaderReverse(pPI, pStart, Flags, (ogp_uint8_t)PacketNo, (ogp_uint8_t)PacketCount);

		OGP_SendTo(pStart2, (ogp_uint32_t)(pEnd - pStart2), pPI->SockAddr, pPI->SockAddrLen, pPI->lpUser);
	}
}

static ogp_uint32_t OGP_CreateErrorMessage(parse_info_t* pPI, EOGP_Error ErrorValue) {
	pPI->pSndBufEnd = pPI->pSndBufStart;		// be sure there is nothing in send buffer

	if (ErrorValue == OGP_Error_InvalidChallenge) {
		pPI->pSndBufStart += 128;
		pPI->pSndBufEnd = pPI->pSndBufStart;

		pPI->Head.Flags.bChallengeNumber = true;

		pPI->pSndBufStart = OGP_CreateHeaderReverse(pPI, pPI->pSndBufStart, pPI->Head.Flags, 0, 1);
		*--pPI->pSndBufStart = ErrorValue;

		memcpy(pPI->pSndBufEnd, pPI->Head.pBodyPos, pPI->pRcvBufEnd - pPI->Head.pBodyPos);
		pPI->pSndBufEnd += pPI->pRcvBufEnd - pPI->Head.pBodyPos;
	} else {
		OGP_SndBufAdd_UINT8(pPI, (ogp_uint8_t)ErrorValue);
	}

	pPI->Head.Type = OGP_QueryType_Error;

	return OGP_Error_None;
}

static ogp_uint32_t OGP_ParsePing(parse_info_t* pPI, const ogp_uint8_t* pBufPos) {
	pPI;
	pBufPos;

	return OGP_Error_None;
}

static EOGP_Error OGP_ParseDefault1(parse_info_t* pPI, const ogp_uint8_t* pBufPos) {
	const ogp_serverdata_t*	pSD = OGP_GetServerData();
	ogp_request_flags_t		RcvRequestFlags;
	ogp_serverinfo_fields_t	RcvServerInfoFields = {0};
	ogp_mod_fields_t		RcvModFields = {0};
	ogp_map_fields_t		RcvMapFields = {0};
	ogp_teamlist_fields_t	RcvTeamListFields = {0};
	ogp_playerlist_fields_t	RcvPlayerListFields = {0};
	ogp_addonlist_fields_t	RcvAddOnListFields = {0};

	if (!pPI->Head.bChallengeNumberValid)
		return OGP_Error_InvalidChallenge;

	OGP_GetVarBitArray(&pBufPos, &RcvRequestFlags, sizeof(RcvRequestFlags));
	if (RcvRequestFlags.bServerInfo)
		OGP_GetVarBitArray(&pBufPos, &RcvServerInfoFields, sizeof(RcvServerInfoFields));
	if (RcvServerInfoFields.bMod)
		OGP_GetVarBitArray(&pBufPos, &RcvModFields, sizeof(RcvModFields));
	if (RcvServerInfoFields.bMap)
		OGP_GetVarBitArray(&pBufPos, &RcvMapFields, sizeof(RcvMapFields));
	if (RcvRequestFlags.bTeamList)
		OGP_GetVarBitArray(&pBufPos, &RcvTeamListFields, sizeof(RcvTeamListFields));
	if (RcvRequestFlags.bPlayerList)
		OGP_GetVarBitArray(&pBufPos, &RcvPlayerListFields, sizeof(RcvPlayerListFields));
	if (RcvRequestFlags.bAddOnList)
		OGP_GetVarBitArray(&pBufPos, &RcvAddOnListFields, sizeof(RcvAddOnListFields));

	if (pBufPos > pPI->pRcvBufEnd)
		return OGP_Error_InvalidQuery;

	{	// Request Flags
		ogp_request_flags_t		SndRequestFlags = RcvRequestFlags;
		OGP_memand(&SndRequestFlags, &pSD->RequestFlags, sizeof(SndRequestFlags));
		SndRequestFlags.bColoredNames = false;

		OGP_SndBufAdd_UINT16(pPI, pSD->GameID);
		OGP_SndBufAdd_VarBitArray(pPI, SndRequestFlags);

		if (SndRequestFlags.bServerInfo) {		// Server Info
			ogp_serverinfo_fields_t		SndServerInfoFields = RcvServerInfoFields;
			OGP_memand(&SndServerInfoFields, &pSD->ServerInfo.Fields, sizeof(SndServerInfoFields));

			OGP_SndBufAdd_VarBitArray(pPI, SndServerInfoFields);

			if (SndServerInfoFields.bGameName)
				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.GameName);
			if (SndServerInfoFields.bServerFlags)
				OGP_SndBufAdd_VarBitArray(pPI, pSD->ServerInfo.ServerFlags);
			if (SndServerInfoFields.bHostName) {
				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.HostName);
				if (SndRequestFlags.bColoredNames) {
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->ServerInfo.HostNameColorInfoLen);
					OGP_SndBufAdd_Buf(pPI, pSD->ServerInfo.HostNameColorInfo, pSD->ServerInfo.HostNameColorInfoLen);
				}
			}
			if (SndServerInfoFields.bConnectPort)
				OGP_SndBufAdd_UINT16(pPI, pSD->ServerInfo.ConnectPort);


			if (SndServerInfoFields.bMod) {	// Mod Info
				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Mod.ModName);

				if (pSD->ServerInfo.Mod.ModName[0]) {
					ogp_mod_fields_t		SndModFields = RcvModFields;
					OGP_memand(&SndModFields, &pSD->ServerInfo.Mod.Fields, sizeof(SndModFields));

					OGP_SndBufAdd_VarBitArray(pPI, SndModFields);

					if (SndModFields.bModIdentifier)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Mod.ModIdentifier);
					if (SndModFields.bModSize)
						OGP_SndBufAdd_UINT32(pPI, pSD->ServerInfo.Mod.ModSize);
					if (SndModFields.bModVersion)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Mod.ModVersion);
					if (SndModFields.bModURL)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Mod.ModURL);
					if (SndModFields.bModAuthor)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Mod.ModAuthor);
				}
			}
			if (SndServerInfoFields.bGameType)
				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.GameType);
			if (SndServerInfoFields.bGameMode)
				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.GameMode);
			if (SndServerInfoFields.bMap) {
				ogp_map_fields_t		SndMapFields = RcvMapFields;
				OGP_memand(&SndMapFields, &pSD->ServerInfo.MapFields, sizeof(SndMapFields));

				OGP_SndBufAdd_VarBitArray(pPI, SndMapFields);

				OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Map.MapName);
				if (SndMapFields.bMapFileName)
					OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Map.MapFileName);
				if (SndMapFields.bMapFileSize)
					OGP_SndBufAdd_UINT32(pPI, pSD->ServerInfo.Map.MapFileSize);
				if (SndMapFields.bMapFileMD5)
					OGP_SndBufAdd_Buf(pPI, pSD->ServerInfo.Map.MapFileMD5, sizeof(pSD->ServerInfo.Map.MapFileMD5));
				if (SndMapFields.bMapVersion)
					OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Map.MapVersion);
				if (SndMapFields.bMapURL)
					OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Map.MapURL);
				if (SndMapFields.bMapAuthor)
					OGP_SndBufAdd_String(pPI, pSD->ServerInfo.Map.MapAuthor);

				if (SndServerInfoFields.bNextMap) {
					OGP_SndBufAdd_String(pPI, pSD->ServerInfo.NextMap.MapName);
					if (SndMapFields.bMapFileName)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.NextMap.MapFileName);
					if (SndMapFields.bMapFileSize)
						OGP_SndBufAdd_UINT32(pPI, pSD->ServerInfo.NextMap.MapFileSize);
					if (SndMapFields.bMapFileMD5)
						OGP_SndBufAdd_Buf(pPI, pSD->ServerInfo.NextMap.MapFileMD5, sizeof(pSD->ServerInfo.NextMap.MapFileMD5));
					if (SndMapFields.bMapVersion)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.NextMap.MapVersion);
					if (SndMapFields.bMapURL)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.NextMap.MapURL);
					if (SndMapFields.bMapAuthor)
						OGP_SndBufAdd_String(pPI, pSD->ServerInfo.NextMap.MapAuthor);
				}
			}

			if (SndServerInfoFields.bPlayerCount)
				OGP_SndBufAdd_VarUINT8_32(pPI, pSD->ServerInfo.PlayerCount);
			if (SndServerInfoFields.bSlotMax)
				OGP_SndBufAdd_VarUINT8_32(pPI, pSD->ServerInfo.SlotMax);
			if (SndServerInfoFields.bBotCount)
				OGP_SndBufAdd_VarUINT8_32(pPI, pSD->ServerInfo.BotCount);
			if (SndServerInfoFields.bReservedSlots)
				OGP_SndBufAdd_VarUINT8_32(pPI, pSD->ServerInfo.ReservedSlots);
		}

		if (SndRequestFlags.bTeamList) {
			ogp_uint32_t	i;

			ogp_teamlist_fields_t		SndTeamListFields = RcvTeamListFields;
			OGP_memand(&SndTeamListFields, &pSD->Team.Fields, sizeof(SndTeamListFields));

			OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Team.Count);
			if (pSD->Team.Count)
				OGP_SndBufAdd_VarBitArray(pPI, SndTeamListFields);
			
			for (i=0; i<pSD->Team.Count; ++i) {
				if (SndTeamListFields.bTeamName) {
					OGP_SndBufAdd_String(pPI, pSD->Team.List[i].Name);
					if (SndRequestFlags.bColoredNames) {
						OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Team.List[i].NameColorInfoLen);
						OGP_SndBufAdd_Buf(pPI, pSD->Team.List[i].NameColorInfo, pSD->Team.List[i].NameColorInfoLen);
					}
				}
				if (SndTeamListFields.bTeamScore)
					OGP_SndBufAdd_VarSINT8_32(pPI, pSD->Team.List[i].Score);
				if (SndTeamListFields.bTeamAveragePing)
					OGP_SndBufAdd_UINT16(pPI, pSD->Team.List[i].AveragePing);
				if (SndTeamListFields.bTeamAverageLoss)
					OGP_SndBufAdd_UINT16(pPI, pSD->Team.List[i].AverageLoss);
				if (SndTeamListFields.bTeamPlayerCount)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Team.List[i].PlayerCount);
				if (SndTeamListFields.bTeamColor)
					OGP_SndBufAdd_UINT16(pPI, pSD->Team.List[i].Color);
			}
		}

		if (SndRequestFlags.bPlayerList) {
			ogp_uint32_t	i;

			ogp_playerlist_fields_t		SndPlayerListFields = RcvPlayerListFields;
			OGP_memand(&SndPlayerListFields, &pSD->Player.Fields, sizeof(SndPlayerListFields));
			
			OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.Count);
			if (pSD->Player.Count)
				OGP_SndBufAdd_VarBitArray(pPI, SndPlayerListFields);

			for (i=0; i<pSD->Player.Count; ++i) {
				if (SndPlayerListFields.bPlayerFlags)
					OGP_SndBufAdd_VarBitArray(pPI, pSD->Player.List[i].Flags);
				if (SndPlayerListFields.bPlayerSlot)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].Slot);
				if (SndPlayerListFields.bPlayerName) {
					OGP_SndBufAdd_String(pPI, pSD->Player.List[i].Name);
					if (SndRequestFlags.bColoredNames) {
						OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].NameColorInfoLen);
						OGP_SndBufAdd_Buf(pPI, pSD->Player.List[i].NameColorInfo, pSD->Player.List[i].NameColorInfoLen);
					}
				}
				if (SndPlayerListFields.bPlayerTeam)
					OGP_SndBufAdd_VarSINT8_32(pPI, pSD->Player.List[i].TeamNo);
				if (SndPlayerListFields.bPlayerClass)
					OGP_SndBufAdd_String(pPI, pSD->Player.List[i].Class);
				if (SndPlayerListFields.bPlayerRace)
					OGP_SndBufAdd_String(pPI, pSD->Player.List[i].Race);

				if (SndPlayerListFields.bPlayerScore)
					OGP_SndBufAdd_VarSINT8_32(pPI, pSD->Player.List[i].Score);
				if (SndPlayerListFields.bPlayerFrags)
					OGP_SndBufAdd_VarSINT8_32(pPI, pSD->Player.List[i].Frags);
				if (SndPlayerListFields.bPlayerKills)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].Kills);
				if (SndPlayerListFields.bPlayerDeath)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].Death);
				if (SndPlayerListFields.bPlayerSuicides)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].Suicides);
				if (SndPlayerListFields.bPlayerTeamKills)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].TeamKills);

				if (SndPlayerListFields.bPlayerID)
					OGP_SndBufAdd_UINT32(pPI, pSD->Player.List[i].ID);
				if (SndPlayerListFields.bPlayerGlobalID)
					OGP_SndBufAdd_String(pPI, pSD->Player.List[i].GlobalID);
				if (SndPlayerListFields.bPlayerPing)
					OGP_SndBufAdd_UINT16(pPI, pSD->Player.List[i].Ping);
				if (SndPlayerListFields.bPlayerLoss)
					OGP_SndBufAdd_UINT16(pPI, pSD->Player.List[i].Loss);
				if (SndPlayerListFields.bPlayerModel)
					OGP_SndBufAdd_String(pPI, pSD->Player.List[i].Model);
				if (SndPlayerListFields.bPlayerTime)
					OGP_SndBufAdd_UINT16(pPI, pSD->Player.List[i].Time);

				if (SndPlayerListFields.bPlayerAddress) {
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Player.List[i].AddrLen);
					OGP_SndBufAdd_Buf(pPI, pSD->Player.List[i].Addr, pSD->Player.List[i].AddrLen);
				}
			}
		}

		if (SndRequestFlags.bRuleList) {
			ogp_uint32_t	i;
			
			OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Rule.Count);

			for (i=0; i<pSD->Rule.Count; ++i) {
				OGP_SndBufAdd_String(pPI, pSD->Rule.List[i].Key);
				OGP_SndBufAdd_String(pPI, pSD->Rule.List[i].Value);
			}
		}

		if (SndRequestFlags.bAddOnList) {
			ogp_uint32_t	i;

			ogp_addonlist_fields_t		SndAddOnListFields = RcvAddOnListFields;
			OGP_memand(&SndAddOnListFields, &pSD->AddOn.Fields, sizeof(SndAddOnListFields));

			OGP_SndBufAdd_VarUINT8_32(pPI, pSD->AddOn.Count);
			if (pSD->AddOn.Count)
				OGP_SndBufAdd_VarBitArray(pPI, SndAddOnListFields);
			
			for (i=0; i<pSD->AddOn.Count; ++i) {
				if (SndAddOnListFields.bAddOnFlags)
					OGP_SndBufAdd_VarBitArray(pPI, pSD->AddOn.List[i].Flags);
				if (SndAddOnListFields.bAddOnShortName)
					OGP_SndBufAdd_String(pPI, pSD->AddOn.List[i].ShortName);
				if (SndAddOnListFields.bAddOnLongName)
					OGP_SndBufAdd_String(pPI, pSD->AddOn.List[i].LongName);
				if (SndAddOnListFields.bAddOnVersion)
					OGP_SndBufAdd_String(pPI, pSD->AddOn.List[i].Version);
			}
		}

		if (SndRequestFlags.bLimitList) {
			ogp_uint32_t	i;

			OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Limit.Count);
			
			for (i=0; i<pSD->Limit.Count; ++i) {
				OGP_SndBufAdd_VarBitArray(pPI, pSD->Limit.List[i].LimitType);
				OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Limit.List[i].Limit);
				if (pSD->Limit.List[i].LimitType.bLimitLeft)
					OGP_SndBufAdd_VarUINT8_32(pPI, pSD->Limit.List[i].Left);
			}
		}
	}

	return OGP_Error_None;
}

static ogp_uint32_t OGP_ParseHeader(parse_info_t* pPI, const ogp_uint8_t* pBufPos) {
	ogp_uint32_t ChallengeNo = 0;

	pPI->Head.MaxBytesPerPacket = OGP_DEFAUT_MAX_BYTES_PER_PACKET;
	pPI->Head.RequestID = 0;
	
	pPI->Head.pBodyPos = pBufPos;
	pPI->Head.pBodyPos += pPI->Head.HeadLen = OGP_GetUINT8(&pBufPos);
	if (pPI->Head.pBodyPos > pPI->pRcvBufEnd) // packet too short
		return OGP_Error_DontSend;

	pPI->Head.Type = OGP_GetUINT8(&pBufPos);

	OGP_GetVarBitArray(&pBufPos, &pPI->Head.Flags, sizeof(pPI->Head.HeadLen));
	if (pPI->Head.Flags.bAnswer)
		return OGP_Error_DontSend;

	if (pPI->Head.Flags.bChallengeNumber)
		ChallengeNo = OGP_GetUINT32(&pBufPos);

	if (pPI->Head.Flags.bRequestID)
		pPI->Head.RequestID = OGP_GetUINT32(&pBufPos);

	if (pPI->Head.Flags.bSplit) {		
		pPI->Head.MaxBytesPerPacket = OGP_GetUINT16(&pBufPos);

		if (pPI->Head.MaxBytesPerPacket < 500)
			return OGP_Error_InvalidValue;
	}
	
#if OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_CHAR
	// UTF8 is not required in char encoding, so deactivate it
	pPI->Head.Flags.bUTF8 = false;
#endif

	if (pBufPos > pPI->Head.pBodyPos)
		return OGP_Error_DontSend;		// Invalid header
	
	pPI->Head.ChallengeNumber = OGP_GetChallengeNo(pPI->SockAddr, pPI->SockAddrLen);
	pPI->Head.bChallengeNumberValid = (pPI->Head.ChallengeNumber == ChallengeNo);

	if (pPI->Banned) {
		pPI->Head.Flags.bChallengeNumber = false;
		return OGP_Error_Banned;
	}

	switch (pPI->Head.Type) {
		case OGP_QueryType_Ping:
			return OGP_ParsePing(pPI, pPI->Head.pBodyPos);
		case OGP_QueryType_Default1:
			return OGP_ParseDefault1(pPI, pPI->Head.pBodyPos);

		case OGP_QueryType_Error:
			return OGP_Error_DontSend;

		default:
			return OGP_Error_InvalidType;
	}

	return OGP_Error_DontSend;
}

int OGP_ReceiveFrom(const void* __pBuf, size_t BufLen, void* SockAddr, int SockAddrLen, void* lpUser) {
	const unsigned char* pBuf = __pBuf;

	// OGP header check	
	if (BufLen < 8)
		return false;
	if (*(ogp_uint32_t*)pBuf != 0xFFFFFFFFu)
		return false;
	if (*(ogp_uint32_t*)(pBuf+4) != *(ogp_uint32_t*)"OGP\0")
		return false; 

	if (BufLen > OGP_MAX_RECEIVE_SIZE)
		return true;
	if (SockAddrLen < 4)
		return true;

	{
		parse_info_t	PI;
		ogp_uint32_t		SendErr = OGP_Error_None;
		ogp_uint8_t*		RcvBuf;

		PI.Banned = OGP_IsBanned(SockAddr, SockAddrLen);
		if (PI.Banned == OGP_Banned_Ignore)
			return true;

		PI.lpUser = lpUser;
		PI.SockAddr = SockAddr;
		PI.SockAddrLen = SockAddrLen;

		// extra large own receive buffer to avoid faults caused by invalid packets
		PI.pRcvBufStart = RcvBuf = (ogp_uint8_t*)alloca(BufLen + 32);
		PI.pRcvBufEnd = PI.pRcvBufStart + BufLen;
		memcpy(RcvBuf, pBuf, BufLen);
		memset(RcvBuf + BufLen, 0, 32);

		/* SndBuf architecture
		 * *--------------------------------------------------------------------------*
		 * | Secure | OGP Header |                OGP Body                   | Secure |
		 * *--------------------------------------------------------------------------*
		 *          ^            ^                                           ^    
		 *      pSndBufMin  pSndBufStart                                pSendBufMax  
		 *                   pSndBufEnd                                      
		 *
		 * The body is generated first by increasing pSndBufEnd
		 * The header is generated later in reverse order by decreasing pSndBufStart
		 */
		PI.pSndBufMin = (ogp_uint8_t*)alloca(OGP_MAX_ANSWER_HEADER_LENGTH*2 + OGP_MAX_PACKET_LEN + 128);
		PI.pSndBufMin += OGP_MAX_ANSWER_HEADER_LENGTH;
		PI.pSndBufEnd = PI.pSndBufStart = PI.pSndBufMin + OGP_MAX_ANSWER_HEADER_LENGTH;
		PI.pSndBufMax = PI.pSndBufStart + OGP_MAX_PACKET_LEN;

		SendErr = OGP_ParseHeader(&PI, PI.pRcvBufStart + 8);

		if (PI.Banned && SendErr != OGP_Error_Banned)
			return true;		// Ignore if syntax of ogp header is incorrect		

		if (SendErr != OGP_Error_DontSend && SendErr != OGP_Error_None)
			SendErr = OGP_CreateErrorMessage(&PI, (ogp_uint8_t)SendErr);

		if (SendErr == OGP_Error_None)
			OGP_Send(&PI);
	}

	return true;
}


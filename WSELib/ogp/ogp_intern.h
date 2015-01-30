#ifndef __OGP_INTERN_H__
#define __OGP_INTERN_H__

#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include "ogp_basictypes.h"
#include "ogp.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __cplusplus

#define		false		0
#define		true		1

#endif

#if defined _MSC_VER
# define		OGP_INLINE			__inline
#elif defined __GNUC__
# define		OGP_INLINE			__inline__
#else
# error Unknown compiler
#endif

typedef struct {
	unsigned char bAnswer : 1;
	unsigned char bChallengeNumber : 1;
	unsigned char bRequestID : 1;
	unsigned char bSplit : 1;
	unsigned char bUTF8 : 1;
	unsigned char Reserved : 4;	
} ogp_header_flags_t;


typedef struct {
	void*	lpUser;
	void*	SockAddr;
	int		SockAddrLen;

	const ogp_uint8_t*	pRcvBufStart;
	const ogp_uint8_t*	pRcvBufEnd;

	ogp_uint8_t*	pSndBufMin;
	ogp_uint8_t*	pSndBufStart;
	ogp_uint8_t*	pSndBufEnd;
	ogp_uint8_t*	pSndBufMax;

	EOGP_Banned		Banned;

	struct {
		ogp_uint8_t	HeadLen;

		EOGP_QueryType	Type;

		ogp_header_flags_t	Flags;

		ogp_uint16_t	MaxBytesPerPacket;
		ogp_uint32_t	RequestID;
		ogp_uint32_t	ChallengeNumber;
		ogp_uint8_t		bChallengeNumberValid;

		const ogp_uint8_t*		pBodyPos;
	} Head;
} parse_info_t;


// Identifier + HeadSize + Type + HeadFlags + ChallengeNumber + RequestID + Split
#define		OGP_MAX_ANSWER_HEADER_LENGTH	(8 + 1 + 1 + sizeof(ogp_header_flags_t) + 4 + 4 + 2)


extern void OGP_strncpy_AsciiToUnicode(wchar_t *dst, const char *src, size_t n);
extern size_t OGP_UnicodeToUTF8_GetLength(const wchar_t *pStr);
extern void OGP_strncpy_UnicodeToUTF8(char *dst, const wchar_t *src, size_t n);
extern void OGP_strncpy_UnicodeToAscii(char *dst, const wchar_t *src, size_t n);
extern void OGP_strncpy_AsciiToUTF8(char *dst, const char *src, size_t n);
extern void OGP_strncpy_UTF8ToUnicode(wchar_t *dst, const char *src, size_t n);
extern size_t OGP_UTF8_GetLength(const char* pStr);
extern void OGP_strncpy_UTF8ToAscii(char *dst, const char *src, size_t n);




static OGP_INLINE void OGP_SndBufAdd_UINT8(parse_info_t* pPI, ogp_uint8_t Value) {
	*pPI->pSndBufEnd++ = Value;
}

static OGP_INLINE void OGP_SndBufAdd_UINT16(parse_info_t* pPI, ogp_uint16_t Value) {
	*(ogp_uint16_t*)pPI->pSndBufEnd = Value;
	pPI->pSndBufEnd += sizeof(Value);
}

static OGP_INLINE void OGP_SndBufAdd_UINT32(parse_info_t* pPI, ogp_uint32_t Value) {
	*(ogp_uint32_t*)pPI->pSndBufEnd = Value;
	pPI->pSndBufEnd += sizeof(Value);
}

#define OGP_SndBufAdd_VarBitArray(pPI, Data)	OGP_SetVarBitArray(&pPI->pSndBufEnd,&Data, sizeof(Data))

void OGP_SndBufAdd_VarUINT8_32(parse_info_t* pPI, ogp_uint32_t Value);
void OGP_SndBufAdd_VarSINT8_32(parse_info_t* pPI, ogp_int32_t Value);
void OGP_SndBufAdd_Buf(parse_info_t* pPI, const ogp_uint8_t* pAddBuf, ogp_uint32_t AddBufLen);

// Can create a answer AND query header
ogp_uint8_t* OGP_CreateHeaderReverse(parse_info_t* pPI, ogp_uint8_t* pBuf, ogp_header_flags_t Flags, ogp_uint8_t PacketNo, ogp_uint8_t PacketCount);
void OGP_SetVarBitArray(ogp_uint8_t** pBuf, const void* pData, ogp_uint32_t DataLen);
ogp_uint8_t* OGP_SetReverseVarBitArray(ogp_uint8_t* pBuf, const void* pData, ogp_uint32_t DataLen);

void OGP_memand(void* pDest, const void* pSource, int size);


#ifdef __cplusplus
}
#endif

#endif

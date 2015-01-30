
#include "ogp_intern.h"

#include <wchar.h>
#include <limits.h>


void OGP_strncpy_AsciiToUnicode(wchar_t *dst, const char *src, size_t n) {
	char c;
	while (--n && (c = *src++) != '\0') {
		*dst++ = (unsigned char)c;
	}
	*dst++ = '\0';
}

void OGP_strncpy_UnicodeToAscii(char *dst, const wchar_t *src, size_t n) {
	wchar_t c;
	while (--n && (c = *src++) != '\0') {
		*dst++ = (c > UCHAR_MAX) ? '?' : (char)c;
	}
	*dst++ = '\0';
}

__inline int UTF8_CharEncodedLength(unsigned c) {
	if (c < 0x80) {
		// 0xxxxxxx
		return 1;
	} else if (c < (1 << 11)) {
		// 110xxxxx 10xxxxxx
		return 2;
	} else if (sizeof(wchar_t) == 2 || c < (1 << 16)) {
		// 1110xxxx 10xxxxxx 10xxxxxx
		return 3;
	} else {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		return 4;
	}
}

__inline void UTF8_EncodeChar(char** str, unsigned c) {
	if (c < 0x80) {
		// 0xxxxxxx
		*(*str)++ = (char)c;
	} else if (c < (1 << 11)) {
		// 110xxxxx 10xxxxxx
		*(*str)++ = 0xC0 | ((c >>  6) /*& 0x1F*/);
		*(*str)++ = 0x80 | ((c >>  0) & 0x3F);
	} else if (c < (1 << 16)) {
		// 1110xxxx 10xxxxxx 10xxxxxx
		*(*str)++ = 0xE0 | ((c >> 12) /*& 0x0F*/);
		*(*str)++ = 0x80 | ((c >>  6) & 0x3F);
		*(*str)++ = 0x80 | ((c >>  0) & 0x3F);
	} else {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		*(*str)++ = 0xF0 | ((c >> 18) & 0x07);
		*(*str)++ = 0x80 | ((c >> 12) & 0x3F);
		*(*str)++ = 0x80 | ((c >>  6) & 0x3F);
		*(*str)++ = 0x80 | ((c >>  0) & 0x3F);
	}
}

__inline unsigned UTF8_DecodeChar(const char** __str) {
	const unsigned char** str = (const unsigned char**)__str;
	unsigned c;

	if (((*str)[0] & 0x80) == 0x00) {
		// 0xxxxxxx
		c = (*str)[0] & 0x7F;
		*str += 1;
	} else if (((*str)[0] & 0xE0) == 0xC0 && ((*str)[1] & 0xC0) == 0x80) {
		// 110xxxxx 10xxxxxx
		c = ((*str)[1] & 0x3F) | ((*str)[0] & 0x1F) << 6;
		*str += 2;
	} else if (((*str)[0] & 0xF0) == 0xE0 && ((*str)[1] & 0xC0) == 0x80 && ((*str)[2] & 0xC0) == 0x80) {
		// 1110xxxx 10xxxxxx 10xxxxxx
		c = ((*str)[2] & 0x3F) | ((*str)[1] & 0x3F) << 6 | ((*str)[0] & 0x0F) << 12;
		*str += 3;
	} else if (((*str)[0] & 0xF8) == 0xF0 && ((*str)[1] & 0xC0) == 0x80 && ((*str)[2] & 0xC0) == 0x80 && ((*str)[3] & 0xC0) == 0x80) {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		c = ((*str)[3] & 0x3F) | ((*str)[2] & 0x3F) << 6 | ((*str)[1] & 0x3F) << 12 | ((*str)[0] & 0x07) << 18;
		*str += 4;
	} else {
		// Invalid
		c = '?';
		*str += 1;
	}
	
	return c;
}

__inline unsigned UTF8_DecodeCharLength(const char* __str) {
	const unsigned char* str = __str;

	if ((str[0] & 0x80) == 0x00) {
		// 0xxxxxxx
		return 1;
	} else if ((str[0] & 0xE0) == 0xC0 && (str[1] & 0xC0) == 0x80) {
		// 110xxxxx 10xxxxxx
		return 2;
	} else if ((str[0] & 0xF0) == 0xE0 && (str[1] & 0xC0) == 0x80 && (str[2] & 0xC0) == 0x80) {
		// 1110xxxx 10xxxxxx 10xxxxxx
		return 3;
	} else if ((str[0] & 0xF8) == 0xF0 && (str[1] & 0xC0) == 0x80 && (str[2] & 0xC0) == 0x80 && (str[3] & 0xC0) == 0x80) {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		return 4;
	} else {
		// Invalid
		return 1;
	}
}

size_t OGP_UTF8_GetLength(const char* pStr) {
	size_t n = 0;

	while (*pStr != '\0') {
		pStr += UTF8_DecodeCharLength(pStr);
		++n;
	}

	return n;
}

size_t OGP_UnicodeToUTF8_GetLength(const wchar_t *pStr) {
	size_t n = 0;
	wchar_t c;
	
	while ((c = *pStr++) != '\0')
		n += UTF8_CharEncodedLength(c);

	return n;
}

void OGP_strncpy_UnicodeToUTF8(char *dst, const wchar_t *src, size_t n) {
	wchar_t c;
	while (--n && (c = *src++) != '\0')
		UTF8_EncodeChar(&dst, c);
	*dst++ = '\0';
}

void OGP_strncpy_AsciiToUTF8(char *dst, const char *src, size_t n) {
	unsigned char c;
	while (--n && (c = *src++) != '\0')
		UTF8_EncodeChar(&dst, c);
	*dst++ = '\0';
}

void OGP_strncpy_UTF8ToUnicode(wchar_t *dst, const char *src, size_t n) {
	unsigned c;
	while (--n && (c = UTF8_DecodeChar(&src)) != '\0') {
		*dst++ = (c > WCHAR_MAX) ? '?' : c;
	}
	*dst++ = '\0';
}

void OGP_strncpy_UTF8ToAscii(char *dst, const char *src, size_t n) {
	unsigned c;
	while (--n && (c = UTF8_DecodeChar(&src)) != '\0') {
		*dst++ = (c > UCHAR_MAX) ? '?' : c;
	}
	*dst++ = '\0';
}


void OGP_SndBufAdd_VarUINT8_32(parse_info_t* pPI, ogp_uint32_t Value) {
	if (Value < 0xFE) {
		OGP_SndBufAdd_UINT8(pPI, (ogp_uint8_t)Value);
	} else if (Value < 0xFFFF) {
		OGP_SndBufAdd_UINT8(pPI, 0xFE);
		OGP_SndBufAdd_UINT16(pPI, (ogp_uint16_t)Value);
	} else {
		OGP_SndBufAdd_UINT8(pPI, 0xFF);
		OGP_SndBufAdd_UINT32(pPI, Value);
	}
}

void OGP_SndBufAdd_VarSINT8_32(parse_info_t* pPI, ogp_int32_t Value) {
	if (-0x7E <= Value && Value <= 0x7F) {
		OGP_SndBufAdd_UINT8(pPI, (ogp_uint8_t)(ogp_int8_t)Value);
	} else if (-0x7FFF <= Value && Value <= 0x7FFF) {
		OGP_SndBufAdd_UINT8(pPI, (ogp_uint8_t)-0x80);
		OGP_SndBufAdd_UINT16(pPI, (ogp_uint16_t)(ogp_int16_t)Value);
	} else {
		OGP_SndBufAdd_UINT8(pPI, (ogp_uint8_t)-0x7F);
		OGP_SndBufAdd_UINT32(pPI, Value);
	}
}

void OGP_SndBufAdd_Buf(parse_info_t* pPI, const ogp_uint8_t* pAddBuf, ogp_uint32_t AddBufLen) {
	if (pPI->pSndBufEnd + AddBufLen >= pPI->pSndBufMax) {
		pPI->pSndBufEnd = pPI->pSndBufMax;
		return;
	}

	memcpy(pPI->pSndBufEnd, pAddBuf, AddBufLen);
	pPI->pSndBufEnd += AddBufLen;
}

// Can create a answer AND query header
ogp_uint8_t* OGP_CreateHeaderReverse(parse_info_t* pPI, ogp_uint8_t* pBuf, ogp_header_flags_t Flags, ogp_uint8_t PacketNo, ogp_uint8_t PacketCount) {
	ogp_uint8_t* pStartBuf = pBuf;

	if (Flags.bSplit) {
		if (Flags.bAnswer) {
			*--pBuf = PacketNo;
			*--pBuf = PacketCount;
		} else {
			pBuf -= 2;
			*(ogp_uint16_t*)pBuf = pPI->Head.MaxBytesPerPacket;
		}
	}

	if (Flags.bRequestID) {
		pBuf -= 4;
		*(ogp_uint32_t*)pBuf = pPI->Head.RequestID;
	}

	if (Flags.bChallengeNumber) {
		pBuf -= 4;
		*(ogp_uint32_t*)pBuf = pPI->Head.ChallengeNumber;
	}

	pBuf = OGP_SetReverseVarBitArray(pBuf, &Flags, sizeof(Flags));

	*--pBuf = pPI->Head.Type;

	// Splitted because VS optimizer got problems with "*--pBuf = (UINT8)(pStartBuf - pBuf) + 1;"
	--pBuf;
	*pBuf = (ogp_uint8_t)(pStartBuf - pBuf);		// HeadLen

	pBuf -= 4;
	*(ogp_uint32_t*)pBuf = *(ogp_uint32_t*)"OGP\0";

	pBuf -= 4;
	*(ogp_uint32_t*)pBuf = 0xFFFFFFFFu;

	return pBuf;
}

void OGP_SetVarBitArray(ogp_uint8_t** pBuf, const void* pData, ogp_uint32_t DataLen) {
	ogp_uint32_t Len;
	ogp_uint32_t i;

	for (Len = DataLen; Len > 1 && *((ogp_uint8_t*)pData+Len-1) == 0; --Len)
		;

	for (i=0; i<Len-1; ++i)
		*(*pBuf)++ = *((ogp_uint8_t*)pData+i) | 0x80;
	*(*pBuf)++ = *((ogp_uint8_t*)pData + Len-1) & 0x7F;
}

ogp_uint8_t* OGP_SetReverseVarBitArray(ogp_uint8_t* pBuf, const void* pData, ogp_uint32_t DataLen) {
	int Len;
	int i;

	for (Len = DataLen; Len > 1 && *((ogp_uint8_t*)pData+Len-1) == 0; --Len)
		;

	*--pBuf = *((ogp_uint8_t*)pData + Len-1) & 0x7F;
	for (i=Len-2; i>=0; --i)
		*--pBuf = *((ogp_uint8_t*)pData+i) | 0x80;

	return pBuf;
}


void OGP_memand(void* pDest, const void* pSource, int size) {
	ogp_uint8_t* pDest2 = (ogp_uint8_t*)pDest;
	const ogp_uint8_t* pSource2 = (const ogp_uint8_t*)pSource;

	for (; size; --size)
		*pDest2++ &= *pSource2++;
}


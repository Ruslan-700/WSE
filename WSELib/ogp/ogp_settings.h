#ifndef __OGP_SETTINGS_H__
#define __OGP_SETTINGS_H__

#define		OGP_CHALLENGE_TIMEOUT				(1*60*60)	// Timeout of challenge number in seconds
#define		OGP_MAX_RECEIVE_SIZE				500
#define		OGP_DEFAUT_MAX_BYTES_PER_PACKET		0

#define		OGP_MAX_PACKET_LEN					65536


#define		OGP_CHAR_ENCODING_CHAR		0		// 8-bit character encoding, data is sent in latin1 charset
#define		OGP_CHAR_ENCODING_WCHAR		1		// wide character encoding (wchar_t), data is sent in utf8
#define		OGP_CHAR_ENCODING_UTF8		2		// utf8 charset encoding, data is sent in utf8

//! Indicates the character encoding:
#ifndef OGP_CHAR_ENCODING
	#define		OGP_CHAR_ENCODING		OGP_CHAR_ENCODING_UTF8
#endif


//! Controls the dimensions of the array in the server data struct
#define		OGP_LIMITLIST_MAX		1
#define		OGP_ADDONLIST_MAX		1
#define		OGP_RULELIST_MAX		256
#define		OGP_TEAMLIST_MAX		8
#define		OGP_PLAYERLIST_MAX		250


#if OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_CHAR

# define		OGP_CHAR			char
# define		OGP_TEXT(x)			x

#elif OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_WCHAR

# define		OGP_CHAR			wchar_t
# define		OGP_TEXT(x)			L ## x

#elif OGP_CHAR_ENCODING == OGP_CHAR_ENCODING_UTF8

# define		OGP_CHAR			char
# define		OGP_TEXT(x)			x

#else
#	error Invalid OGP_CHAR_ENCODING
#endif

#endif

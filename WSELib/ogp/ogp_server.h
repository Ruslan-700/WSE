#ifndef __OGP_SERVER_H__
#define __OGP_SERVER_H__

#include "ogp.h"

#ifdef __cplusplus
extern "C" {
#endif

// Callback functions
extern void OGP_SendTo(const void* pBuf, size_t BufLen, void* SockAddr, int SockAddrLen, void* lpUser);
extern void OGP_UpdateServerData(ogp_serverdata_t* pServerData);
extern EOGP_Banned OGP_IsBanned(void* SockAddr, int SockAddrLen);

// Returns 1 if it is a udp packet for ogp oder false if not
// SockAddr should contain a sockaddr structure of 3.3BSD-Based-Systems
int OGP_ReceiveFrom(const void* pBuf, size_t BufLen, void* SockAddr, int SockAddrLen, void* lpUser); 


#ifdef __cplusplus
}
#endif

#endif

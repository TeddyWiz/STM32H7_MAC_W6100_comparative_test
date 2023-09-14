#ifndef _LOOPBACK_H_
#define _LOOPBACK_H_

#include <stdint.h>

/* Loopback test debug message printout enable */
//#define	_LOOPBACK_DEBUG_

/* DATA_BUF_SIZE define for Loopback example */
#ifndef DATA_BUF_SIZE
	#define DATA_BUF_SIZE			8000//16384//2048
#endif

int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port, uint8_t loopback_mode);
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port, uint8_t loopback_mode);
int32_t loopback_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport, uint8_t loopback_mode);
int32_t iperf_tcpc(uint8_t sn, uint8_t* buf, uint8_t* destip, uint16_t destport, uint32_t size, uint16_t count, uint8_t loopback_mode);
int8_t tcps_status(uint8_t sn, uint16_t port, uint8_t TCP_mode);
#endif

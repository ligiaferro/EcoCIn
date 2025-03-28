#ifndef __SNPES_TYPES_H__
#define __SNPES_TYPES_H__

#include <stdint.h>
#include "snpes_cfg.h"
#include "CircularQueue.h"
#include "MemoryManager.h"
#include "ConnInterface.h"
#include "TimerInterface.h"

typedef enum { GATEWAY = 0x00, NODE = 0xFF } DeviceType_t;

typedef enum {
	SEND_INFO = 0x00,
	RECV_SYNC,
	WAIT_ACK,
	RECV_ACK,
	IDLE,
	WAIT_DATA,
	RECV_DATA,
	DATA_AVAIL,
	SEND_TRANS_START,
	WAIT_TRANS_START,
	RECV_FULL,
	SEND_DATA,
	WAIT_DATA_ACK
} States_t;

typedef enum { NOT_CONNETED = 0x00, CONNECTED, CONNECTING } ConnState_t;

typedef enum {
	SCAN = 0x00,
	INFO,
	SYNC,
	ACK,
	FULL,
	DATA,
	ALIVE,
	TRANS_START,
	TRANS_RETRY
} PacketType_t;

typedef struct {
	/* client info */
	uint8_t unique_id;
	uint8_t network_id;
	/* connection state */
	States_t state;
	/* flags */
	ConnState_t connected;
	/* time references */
	uint8_t timeout_cnt;
	uint32_t timer_ref;
	uint32_t alive_ref;
	/* recive data buffer */
	void *recv_data;
	uint16_t recv_data_size;
	uint8_t income_pkt_cnt;
	uint8_t in_expt_pkt;
	/* send data buffer */
	void *send_data;
	uint16_t send_data_size;
	uint8_t outgoing_pkt_cnt;
	uint8_t out_expt_pkt;
} ClientCtx_t;

typedef struct {
	LoraItf_t *socket;
	TimerItf_t *timer;
} HwCtx_t;

typedef struct {
	uint8_t unique_id;
	uint8_t network_id;
	HwCtx_t hw;
	Queue_t stream_in;
	Queue_t stream_out;
	MemMgr_t mem;
} DeviceCtx_t;

typedef struct {
	uint8_t src_uid;
	uint8_t src_nid;
	uint8_t dest_uid;
	uint8_t dest_nid;
	uint8_t flgs_seq;
	uint8_t data_size;
	uint8_t data[PKT_SIZE - META_SIZE];
} Packet_t;

#endif /* __SNPES_TYPES_H__ */

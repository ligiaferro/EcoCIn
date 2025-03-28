#ifndef __SNPES_UTILS_H__
#define __SNPES_UTILS_H__

#include "snpes_types.h"
#include <stdint.h>

/**
 * @brief loads given data to a Packet in signal format
 *
 * @param *pkt: pointer to the Packet
 * @param signal: signal flag define at snpes_types
 * @param src_uid: UID of the source
 * @param src_nid: NID of the source
 * @param dest_uid: UID of the destination
 * @param dest_nid: NID of the destination
 * @param seq: Sequence Number of the Packet
 * @param size: Data size
 */
void build_signal(Packet_t *pkt, PacketType_t signal, uint8_t src_uid,
		  uint8_t src_nid, uint8_t dest_uid, uint8_t dest_nid,
		  uint8_t seq, uint8_t size);

/**
 * @brief loads given data to a Packet in data format
 *
 * @param *pkt: pointer to the Packet
 * @param src_uid: UID of the source node
 * @param src_nid: NID of the source node
 * @param dest_uid: UID of the destination node
 * @param dest_nid: NID of the destination node
 * @param seq: Sequence Number of the data Packet
 * @param src: pointer to data source to send in the Packet
 * @param size: the amount of bytes from data source to send
 */
void build_data(Packet_t *pkt, uint8_t src_uid, uint8_t src_nid,
		uint8_t dest_uid, uint8_t dest_nid, uint8_t seq,
		const void *src, uint8_t size);

/**
 * @brief enqueue a Signal Packet to the stream out of a given protocol device
 *
 * @param *dev: pointer to the protocol device
 * @param signal: signal flag define at snpes_types
 * @param dest_uid: UID of the destination node
 * @param dest_nid: NID of the destination node
 * @param seq: sequence number of the signal
 * @param size: Data size
 */
void enqueue_signal(DeviceCtx_t *dev, PacketType_t signal, uint8_t dest_uid,
		    uint8_t dest_nid, uint8_t seq, uint8_t size);

/**
 * @brief enqueue a Data Packet to the stream out of a given protocol device
 *
 * @param *dev: pointer to the protocol device
 * @param dest_uid: UID of the destination node
 * @param dest_nid: NID of the destination node
 * @param seq: Sequence Number of the data Packet
 * @param src: pointer to data source to send in the Packet
 * @param size: the amount of bytes from data source to send
 */
void enqueue_data(DeviceCtx_t *dev, uint8_t dest_uid, uint8_t dest_nid,
		  uint8_t seq, const void *src, uint8_t size);

/**
 * @brief gets the Packet Flag Type of a given Packet Struct
 *
 * @param *pkt: pointer to a Packet Struct in the memory
 * @retval The Packet Type
 */
PacketType_t get_pkt_type(Packet_t *pkt);

/**
 * @brief gets the Packet sequence number of a given Packet Struct
 *
 * @param *pkt: pointer to a Packet Struct in the memory
 * @retval The Packet Sequence Number
 */
uint8_t get_pkt_seq_number(Packet_t *pkt);

#endif /* __SNPES_UTILS_H__ */

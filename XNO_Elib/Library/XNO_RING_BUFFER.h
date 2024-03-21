/* File: RING_BUFFER.h
* Author: Prabhakar
* Company: Eoxys
* Description: Ring buffer related APIs
* Created on:
* Version History
* v1.0 Initial release
*/
#ifndef RINGBUFFER_H
#define RINGBUFFER_H
/*----------------------------------------------------------------------------
* 								Standard library includes
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
* 								User Includes
*----------------------------------------------------------------------------*/
#include <inttypes.h>
/*----------------------------------------------------------------------------
* 								Typedefs
*----------------------------------------------------------------------------*/

struct ring_buff_t {
  uint8_t *bufptr;
  uint16_t tail;
  uint16_t head;
  uint16_t size;
};
typedef struct ring_buff_t RINGBUFF_T;

/*----------------------------------------------------------------------------
* 								Exported Functions
*----------------------------------------------------------------------------*/
/**
  * @brief  To clear the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
void ringbuf_clear(RINGBUFF_T *buffer);
/**
  * @brief  Checks whether the ring buffer is empty or not
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
uint8_t ringbuf_is_empty(RINGBUFF_T *buffer);
/**
  * @brief  Checks whether the ring buffer is full or not
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
uint8_t ringbuf_is_full(RINGBUFF_T *buffer);
/**
  * @brief  Inserting a data in ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @param  data - Data to insert
  * @retval None
  */
void ringbuf_enqueue(RINGBUFF_T *buffer, uint8_t data);
/**
  * @brief  Insert data of specified size in ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @param  data - Data buffer to insert
  * @param  size - size of Data
  * @retval None
  */
void ringbuf_enqueue_arr(RINGBUFF_T *buffer, const uint8_t *data, int size);
/**
  * @brief  Delete a data in ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @param  data - Data to Delete
  * @retval None
  */
int ringbuf_dequeue(RINGBUFF_T *buffer, uint8_t *data);
/**
  * @brief  Delete data of specified size in ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @param  data - Data buffer to insert
  * @param  len - Length of Data
  * @retval None
  */
int ringbuf_dequeue_arr(RINGBUFF_T *buffer, uint8_t *data, int len);
/**
  * @brief  To get the head position of the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
uint16_t ringbuf_getHead(RINGBUFF_T *buffer);
/**
  * @brief  To get the tail position of the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
uint16_t ringbuf_getTail(RINGBUFF_T *buffer);
/**
  * @brief  To get the size of the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
int ringbuf_size(RINGBUFF_T *buffer);
/**
  * @brief  To configure the ring buffer size
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
void ringbuf_config(RINGBUFF_T *buffer,uint8_t *bufptr,uint16_t size);
/**
  * @brief  To get the number of free spaces in the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
int ringbuf_getFreeSize(RINGBUFF_T *buffer);
/**
  * @brief  To get the number of filled spaces in the ring buffer
  * @param  buffer - Pointer to ring buffer structure
  * @retval None
  */
int ringbuf_getFilledSize(RINGBUFF_T *buffer);
#endif /* RINGBUFFER_H */

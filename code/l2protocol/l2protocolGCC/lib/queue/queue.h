#ifndef NES_QUEUE_H
#define NES_QUEUE_H

#include <stdint.h>

typedef struct queue_entry_structure
{
   uint16_t msgType;
   uint8_t  msgPayload[16];
   uint8_t  msgSize;    // [bytes]
} queue_entry;

typedef struct queue_structure
{
   queue_entry  *data;
   uint8_t       len;
   uint8_t       currentIdx;
   uint8_t       lastIdx;
} queue;

void queue_init(queue *q, uint8_t len);
void queue_close(queue *q);
uint8_t queue_isEmpty(queue *q);
void queue_getNext(queue *q, queue_entry *qe);
void queue_append(queue *q, queue_entry *qe);

#endif


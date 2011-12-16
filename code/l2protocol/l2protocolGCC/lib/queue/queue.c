#include "queue.h"
#include <stdlib.h>
#include <string.h>

void queue_init(queue *q, uint8_t len) {

   q->data = (queue_entry*)malloc(sizeof(queue_entry)*len);
   q->len = len;
   q->currentIdx = 0;
   q->lastIdx = 0;
} // end of queue_init

void queue_close(queue *q) {
   uint8_t idx;
  
   if(q==NULL)
      return;

   if(q->data != NULL) {
      for(idx=0; idx<q->len; idx++) {
//         if(q->data[idx].msgPayload != NULL) {
//            free(q->data[idx].msgPayload);
//            q->data[idx].msgPayload = NULL;
//         } // end if
      } // end for

      free(q->data);
      q->data = NULL;
   } // end if

   q->len = 0;
   q->currentIdx = 0;
   q->lastIdx = 0;
} // end of queue_close

uint8_t queue_isEmpty(queue *q) {
   if(q==NULL)
      return 0;

   return (q->currentIdx == q->lastIdx);
} // end of queue_isEmpty

void queue_getNext(queue *q, queue_entry *qe) {
   if(q==NULL)
      return;

   if(q->currentIdx == q->lastIdx) {
      return;
   } // end if

   qe->msgType = q->data[q->currentIdx].msgType;
   //qe->msgPayload = (uint8_t *)malloc(sizeof(uint8_t)*q->data[q->currentIdx].msgSize);
   
   memcpy(qe->msgPayload, 
          q->data[q->currentIdx].msgPayload,
          sizeof(uint8_t)*q->data[q->currentIdx].msgSize);
   //qe->msgPayload = q->data[q->currentIdx].msgPayload;

   qe->msgSize = q->data[q->currentIdx].msgSize;

   q->data[q->currentIdx].msgType = 0;
   //if(q->data[q->currentIdx].msgPayload != NULL) {
   //   free(q->data[q->currentIdx].msgPayload);
   //   q->data[q->currentIdx].msgPayload = NULL;
   //} // end if
   q->data[q->currentIdx].msgSize = 0;

   q->currentIdx++;

   if(q->currentIdx >= q->len)
      q->currentIdx = 0;
} // end of queue_getNext

void queue_append(queue *q, queue_entry *qe) {
   if(q==NULL)
      return;

   q->data[q->lastIdx].msgType = qe->msgType;
   ////q->data[q->lastIdx].msgPayload = qe->msgPayload;
   //q->data[q->lastIdx].msgPayload = (uint8_t *)malloc(sizeof(uint8_t)*qe->msgSize);
   
   memcpy(q->data[q->lastIdx].msgPayload,
          qe->msgPayload, 
          sizeof(uint8_t)*qe->msgSize);

   q->data[q->lastIdx].msgSize = qe->msgSize;

   q->lastIdx++;

   if(q->lastIdx >= q->len)
      q->lastIdx = 0;

   if(q->currentIdx == q->lastIdx) {

//      if(q->data[q->currentIdx].msgPayload != NULL) {
//         free(q->data[q->currentIdx].msgPayload);
//         q->data[q->currentIdx].msgPayload = NULL;
//		}
         // overrun
	 // printf("overrun\n");
	   q->currentIdx++;

   } // end if
} // end of queue_append


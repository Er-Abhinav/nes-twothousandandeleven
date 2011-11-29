// Memory analysis with: valgrind --leak-check=full ./queue_tester.out
//==12482== HEAP SUMMARY:
//==12482==     in use at exit: 0 bytes in 0 blocks
//==12482==   total heap usage: 85 allocs, 85 frees, 2,194 bytes allocated
//==12482== 
//==12482== All heap blocks were freed -- no leaks are possible


#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main (void)
{
printf("hello queue\n");

   queue q1;
   uint8_t q1empty;
   uint8_t str[50];

   queue_entry qe1;
   qe1.msgType = 12345;
   qe1.msgPayload = NULL;
   qe1.msgSize = 0;

   queue_init(&q1, 40);

   qe1.msgType = 1;
   qe1.msgPayload = (uint8_t*)malloc(sizeof(uint8_t)*10);
   sprintf(qe1.msgPayload, "payload");
   qe1.msgSize = 10;

   queue_append(&q1, &qe1);

   if(qe1.msgPayload != NULL) {
      free(qe1.msgPayload);
      qe1.msgPayload != NULL;
   } 

   queue_getNext(&q1, &qe1);

   if(qe1.msgPayload != NULL) {
      free(qe1.msgPayload);
      qe1.msgPayload != NULL;
   } 

   int i;
   for(i=0;i<42; i++)
   {
     q1empty = queue_isEmpty(&q1);
     printf("q1 empty?: %d\n", q1empty);
     printf("curr %d, last %d\n", q1.currentIdx, q1.lastIdx);
     sprintf(str,"hello world string %d", i);

     qe1.msgType = 12345+i;
     qe1.msgPayload = str;
     qe1.msgSize = strlen(str)*sizeof(uint8_t);

     queue_append(&q1, &qe1); 
   }

   q1empty = queue_isEmpty(&q1);
   printf("--q1 empty?: %d\n", q1empty);
     printf("curr %d, last %d\n", q1.currentIdx, q1.lastIdx);


   for(i=0;i<42; i++)
   {
     q1empty = queue_isEmpty(&q1);
     queue_getNext(&q1, &qe1); 
     printf("q1 empty?: %d\t%d \t %s\n", q1empty, qe1.msgType,qe1.msgPayload);
     printf("curr %d, last %d\n", q1.currentIdx, q1.lastIdx);
     if(qe1.msgPayload != NULL) {

	free(qe1.msgPayload);
        qe1.msgPayload = NULL;
     } // end if
   }

   q1empty = queue_isEmpty(&q1);
   printf("--q1 empty?: %d\n", q1empty);
     printf("curr %d, last %d\n", q1.currentIdx, q1.lastIdx);

 queue_close(&q1);

}

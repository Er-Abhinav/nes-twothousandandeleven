
typedef enum {
	UNKNOWN = 0,
	BUSY,
	CLAIMED,
	FREE
} l1_bus_status;

/**
 * Start/Sets timer
 * Checks the bus
 */
l1_bus_status getBus(uint16_t arbitration);
/**
 * @pre Bus has to be Claimed
 */
l1_bus_status pushSendBus(uint8_t len, uint8_t *payload);
/**
 * Checks if further data are queued
 * pushes data to the HW-UART
 *
 * CHECK if message to send is a time message
 */
void callbackMsgReceived(void);


typedef struct {
   uint8_t type : 4;
   uint8_t dest_id : 6;
   uint8_t src_id  : 6;
} l2msgArbitration_t;

typedef struct {
   l2msgArbitration_t head;
   uint8_t size : 4;
   uint8_t crc_size: 4;
} l2msgHeader_t;

typedef struct {
	l2msgHeader_t* header;
	uint8_t* data;
	uint8_t* crc;
} l2msg_t;

uint8_t attachL2Send(l2msg_t *msg, uint8_t priority);
uint8_t initL2(void);

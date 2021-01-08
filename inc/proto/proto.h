#ifndef PROTO_H
#define PROTO_H

// #include <stddef.h>
#include <stdint.h>


enum{
	 READ_BYTE = 0,
	 WRITE_BYTE,
};

typedef struct{
	uint16_t command;
	union{
		struct{
			uint16_t offset;
			uint16_t reserved;
		}read;
		struct{
			uint16_t offset;
			uint8_t value;
			uint8_t mask;
		}write;
	};
} proto_msg;




typedef struct{
	uint16_t command;
	uint8_t errcode;
	uint8_t value;
} response_msg;

enum{
	 SUCCES=0,
	 UNKNOWN_COMMAND,
	 INVALID_OFFSET,
	 INVALID_MSG_SIZE,
};

void read_byte_msg(proto_msg* msg ,uint16_t offset);
void write_byte_msg(proto_msg* msg, uint16_t offset, uint8_t val,uint8_t mask);
void response(proto_msg* msg, response_msg* retval);
uint8_t* get_reg(void);




#endif //!PROTO_H


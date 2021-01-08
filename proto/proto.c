#include "proto.h"


void read_byte_msg(proto_msg* msg ,uint16_t offset)
{

	msg->command=READ_BYTE;
	msg->read.offset=offset;
	return;

}

void write_byte_msg(proto_msg* msg, uint16_t offset, uint8_t val,uint8_t mask)
{
	msg->command = WRITE_BYTE;
	msg->write.offset=offset;
	msg->write.value=val;
	msg->write.mask=mask;
	return;

}

static uint8_t proto_register[1024]={0};

uint8_t* get_reg(void)
{
	return proto_register;
}

void response(proto_msg* msg, response_msg* retval)
{
	if(msg->command==READ_BYTE){
		retval->command=msg->command;
		if(msg->read.offset>1023)
		{
			retval->errcode=INVALID_OFFSET;
			return;
		}
		retval->errcode=SUCCES;
		retval->value=proto_register[msg->read.offset];
		return;

	}
	else if(msg->command==WRITE_BYTE){
		retval->command=msg->command;
		if(msg->write.offset>1023)
		{
			retval->errcode=INVALID_OFFSET;
			return;
		}
		retval->errcode=SUCCES;
	       	uint8_t val = msg->write.value & msg->write.mask;
		proto_register[msg->write.offset]&=~msg->write.mask;
		proto_register[msg->write.offset] |= val;
		retval->value=val;
		return;
	}
	else{
		retval->errcode=UNKNOWN_COMMAND;
	}
}







#include <stdint.h>
void foo(uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4){
    uint32_t a_a=0;
    a_a |= ((uint32_t)b1)<<0;
    a_a |= ((uint32_t)b2)<<8;
    a_a |= ((uint32_t)b3)<<16;
    a_a |= ((uint32_t)b4)<<24;//адрес
}


int main(int argc, char const *argv[])
{


    
    foo(192,168,43,2);
    (43200&0xff00)>>8;
    return 0;
}


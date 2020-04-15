// Работа с битами

#ifndef __BitsH__
#define __BitsH__


///////
#define sbi(reg,bit) reg |= (1<<bit)
#define cbi(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))
///////

#define bit_msk(x) (1 << x)

#define st_bit(byte, nbit) (byte |= bit_msk(nbit))
#define cl_bit(byte, nbit) (byte &= ~bit_msk(nbit))
#define set_bit(byte, nbit, value) ((value != 0) ? (st_bit(byte, nbit)) : (cl_bit(byte, nbit)))

#define bit_is_st(byte, nbit) (byte & bit_msk(nbit))
#define bit_is_cl(byte, nbit) (!bit_is_st(byte, nbit))

#define wait_while_bit_is_st(byte, nbit) {while(bit_is_st(byte, nbit));}
#define wait_while_bit_is_cl(byte, nbit) {while(bit_is_cl(byte, nbit));}

#define LO(x) ((unsigned char)x)
#define HI(x) ((unsigned char)(x>>8))

#endif

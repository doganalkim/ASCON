#include <stdio.h>

typedef unsigned long bit64;

bit64 state[5] = {0}, t[5] = {0} ,constants[16] =      {0xf0, 
                                                        0xe1, 
                                                        0xd2, 
                                                        0xc3, 
                                                        0xb4,
                                                        0xa5,
                                                        0x96,
                                                        0x87,
                                                        0x78,
                                                        0x69,
                                                        0x5a,
                                                        0x4b,
                                                        0x3c,
                                                        0x2d,
                                                        0x1e,
                                                        0x0f };

void add_constant(bit64 state[5], int i, unsigned int times)
{
    state[2] = state[2] ^ constants[12 - times + i];
}

bit64 rotate(bit64 x, unsigned short int r)
{
    return ( x >> r ) ^ ( x << (64 - r ));
}

void linear(bit64 state[5])
{
    state[0] ^=  rotate(state[0], 19) ^ rotate(state[0], 28); 
    state[1] ^=  rotate(state[1], 61) ^ rotate(state[1], 39);
    state[2] ^=  rotate(state[2], 1) ^ rotate(state[2], 6);
    state[3] ^=  rotate(state[3], 10) ^ rotate(state[3], 17);
    state[4] ^=  rotate(state[4], 7) ^ rotate(state[4], 41);  
}

void sbox(bit64 x[5])
{
    x[0] ^= x[4]; x[4] ^= x[3]; x[2] ^= x[1];
    t[0] = x[0]; t[1] = x[1]; t[2] = x[2]; t[3] = x[3]; t[4] = x[4];
    t[0] =~ t[0]; t[1] =~ t[1]; t[2] =~ t[2]; t[3] =~ t[3]; t[4] =~ t[4];
    t[0] &= x[1]; t[1] &= x[2]; t[2] &= x[3]; t[3] &= x[4]; t[4] &= x[0];
    x[0] ^= t[1]; x[1] ^= t[2]; x[2] ^= t[3]; x[3] ^= t[4]; x[4] ^= t[0];
    x[1] ^= x[0]; x[0] ^= x[4]; x[3] ^= x[2]; x[2] =~ x[2];
}

void p(bit64 state[5], unsigned int times)
{
    for(int i = 0; i < times ; i++)
    {
        add_constant(state, i, times);
        sbox(state);
        linear(state);
    }
}

void print_bit64(bit64 x)
{
    printf("%016lx\n", x);
}

void print_state(bit64 state[5])
{
    for( int i = 0; i < 5 ; i++)
        print_bit64(state[i]);
}

void initialize(bit64 state[5], bit64 key[2], bit64 IV, bit64 Nonce[2])
{
    state[0] = IV; state[1] = key[0]; state[2] = key[1]; state[3] = Nonce[0]; state[4] = Nonce[1];
    p(state, 12);
}

void encrypt(bit64 state[5], bit64 data[], int size)
{
    bit64 cipher = 0;
    for( int i = 0; i < size ; i++ ){
        cipher = state[0] ^ data[0]; 
        print_bit64(cipher);
        p(state, 6);
    }
}

int main()
{   
    bit64 key[2] = {0x0, 0x0}, IV = 0x80400c0600000000, Nonce[2] = {0x0, 0x0};

    initialize(state, key, IV, Nonce);

    print_state(state);

    bit64 data[8] = {0};

    encrypt(state, data, 8);

    printf("Size: %lu \n", sizeof(bit64) );
}
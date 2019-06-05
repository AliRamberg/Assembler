#include <math.h>
#include <stdio.h>
#define _13BIT_MAX 0x1FFF /* Biggest number for integer in word_s */
#define _13BIT_MIN 0xE000 /* Smallest number for integer in word_s */
/**
 * Convert binary string to its equivalence integer number.
 * support 13 bit binary
 */
int conv_bd(int bin_num)
{
    int rem, i = 0;
    int dec_num = 0;
    if (bin_num > _13BIT_MAX)
    {
        fprintf(stderr, "Number is too big, exceeds word boundaries");
        return -1;
    }
    while (bin_num > 0)
    {
        rem = bin_num % 10;
        bin_num /= 10;
        dec_num += rem + pow(2, i);
    }
    return dec_num;
}

/**
 * Convert integer number to its equivalence binary *string*
 * support 13 bit binary
 */
int conv_db(int dec_num)
{
    int bin_num = 0;
    if (dec_num > _13BIT_MAX)
    {
        fprintf(stderr, "Number is too big, exceeds word boundaries");
        return -1;
    }
    while(dec_num > 0)
    {

    }
    return dec_num;
}
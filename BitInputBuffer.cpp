#include "BitInputBuffer.h"

/* BIT INPUT ROUTINES. */
#include <stdio.h>
#include <stdlib.h>

// Must be identical to this value on Arithmetic Encoder.cpp
#define inputCode_value_bits 16

/* INITIALIZE BIT INPUT. */
void BitInputBuffer::start( unsigned char* buffer_in )
{
   bits_to_go = 0;/* Buffer starts out with */
   garbage_bits = 0;/* no bits in it. */
   _buffer_in = buffer_in;
   _position = 0;
}


/* INPUT A BIT. */
int BitInputBuffer::input_bit()
{
   int t ;

   if (bits_to_go==0) 
   {
      /* Read the next byte if no */
      buffer = (int)_buffer_in[_position] ;//getc(file_in); /* bits are left in buffer. **/   
      _position++;

      if (buffer==EOF)
      {
         garbage_bits += 1 ;                     /* Return arbitrary bits*/         
         if (garbage_bits>inputCode_value_bits-2)     /* after eof, but check */
         {            
         //   fprintf(stderr,"Bad input flle\n") ; /* for too many such. */
            return -1;
         }
      }
      bits_to_go = 8 ;
   }
   t = buffer&1 ;/* Return the next bit from */
   buffer >>= 1 ;/* the bottom of the byte. */

   bits_to_go -= 1 ;
   return t;
}

/* CLOSES THE INPUT FILE */
void BitInputBuffer::end()
{
}
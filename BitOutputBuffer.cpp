#include "BitOutputBuffer.h"
#include <stdio.h>


void BitOutputBuffer::start( unsigned char *buffer, int size )
{
   buffer_bytes = 0 ;/* Buffer Is empty to start */
   bits_to_go = 8 ;/* with. */

   position = 0;
   out_put_buffer = buffer;
}


void BitOutputBuffer::outputBit( int bit )
{
   buffer_bytes >>= 1;/* put bit In top of buffer.*/
 
  if (bit)
     buffer_bytes |= 0x80 ;
  
  bits_to_go -= 1 ;

  if (bits_to_go==0)
  {
     out_put_buffer[ position ] = buffer_bytes; /* Output buffer if It 1s */
     position++;
     bits_to_go = 8;     /* now full. */
  }
}

/* FLUSH OUT THE LAST BITS. */
unsigned char* BitOutputBuffer::end( int &size )
{ 
    buffer_bytes = buffer_bytes >> bits_to_go ;
//  buffer_bytes >> bits_to_go ;

   out_put_buffer[ position ] = buffer_bytes;

   position++;

   size = position;
   return out_put_buffer;
}
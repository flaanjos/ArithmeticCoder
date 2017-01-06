#include "ArithmeticCoder.h"

/* DECLARATIONS USED FOR ARITHMETIC ENCODING AND DECODING */


/* SIZE OF ARITHMETIC CODE VALUES. l */
#define Code_value_bits 16              /* Number of bits in a code _decoding_value   */
//typedef long code_value ;               /* Type of an arithmetic code _decoding_value */

#define Top_value (((long)1<<Code_value_bits)-1) /* Largest code _decoding_value      */

/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE. l */
#define First_qtr (Top_value/4+1)  /* Point after first quarter */
#define Half (2*First_qtr)         /* Point after first half    */
#define Third_qtr (3*First_qtr)    /* Point after third quarter */




ArithmeticCoder::ArithmeticCoder() 
{
}



ArithmeticCoder::~ArithmeticCoder()
{
}



int ArithmeticCoder::encode( unsigned char* buffer_in, int size_in, unsigned char* buffer_out, int capacity_out )
{
   _model.start() ;
   _output.start( buffer_out, capacity_out );
   start_encoding() ;

   for( int i=0 ; i<size_in ; i++ )
   {
      //if( buffer_in[ i ] == EOF )
      //   break;
      
      int symbol = _model.getSymbol( buffer_in[ i ] );
      encode_symbol( symbol, _model.getCumulativeFrequency() );
      _model.update( symbol ) ;
   }
   encode_symbol( AdaptativeModel::EOF_symbol, _model.getCumulativeFrequency() );
   done_encoding();

   int size_out;
   _output.end( size_out ) ;
   return size_out;
}



int ArithmeticCoder::decode( unsigned char* buffer_in, int size_in, unsigned char* buffer_out, int capacity_out )
{
   _model.start() ;
   _input.start( buffer_in ) ;
   start_decoding() ;


  // FILE *file_out = fopen( "descomprimido.dat", "wb" );
   int position = 0;
   for (;;)
   {
      int ch ;
      int symbol;

      symbol = decode_symbol( _model.getCumulativeFrequency() ) ;

      if ( symbol == AdaptativeModel::EOF_symbol )
      {
         break ;
      }
      ch = _model.getChar( symbol ) ;
      buffer_out[ position ] = ch ; //putc( ch , file_out ) ;
      position++;
      _model.update( symbol ) ;
   }
   _input.end() ;

   return position;
}



void ArithmeticCoder::start_encoding() 
{
  low = 0;
  high = Top_value;   /* Full code range. */
  bits_to_follow = 0; /* No bits to follow next. */
} 



void ArithmeticCoder::encode_symbol( int symbol, int cum_freq[] ) 
{
   // Size of the current code region
   long range; 
   range = (long) (high-low)+1 ;

   // Narrow the code region to that allotted to this symbol.
   high = low + (range*cum_freq[symbol-1])/cum_freq[0]-1; 
   low =  low + (range*cum_freq[symbol])/cum_freq[0];

   // Loop to outLput bits.
   for ( ;; )
   {
      if (high<Half) 
      {
         bit_plus_follow(0);// Output 0 if In low half. 
      }
      else if (low>=Half)
      {
         bit_plus_follow(1); // Output 1 if in high half.
         low -= Half;
         high -= Half; // Subtract offset to top. 
      }
      else if (low>=First_qtr && high<Third_qtr)// Output an opposite bit 
      {                                         // later if in middle half. 
         bits_to_follow += 1;
         low -= First_qtr;// Subtract offset to middle
         high -= First_qtr;
      }
      else break;// Otherwise exit loop. 
      low = 2*low;
      high = 2*high+1;// Scale up code range. 
   }
}



void ArithmeticCoder::done_encoding()
{
   bits_to_follow += 1; // Output two bits that 

   if (low<First_qtr) 
   {
      bit_plus_follow(0); // Select the quarter that 
   }
   else
   {
      bit_plus_follow(1);// the current code range contains. 
   }
}



/* OUTPUT BITS PLUS FOLLOWING OPPOSITE BITS. */
/* int bit;*/
void ArithmeticCoder::bit_plus_follow( int bit )
{
  _output.outputBit( bit ) ;

  while ( bits_to_follow > 0 )
  {
     _output.outputBit ( !bit ) ;
     bits_to_follow -= 1 ;
  }
}





void ArithmeticCoder::start_decoding()
{
   int i ;

   _decoding_value = 0 ;

   for ( i = 1 ; i<=Code_value_bits ; i++)/* Input bits to flll the */
   {                                      /* code _decoding_value. */
      _decoding_value =  2*_decoding_value + _input.input_bit() ;
   }
   low = 0 ;
   high = Top_value ; /* Full code ranpe. */
}



/* DECODE THE NEXT SYMBOl.  */
/*int cum_freq[]  Cumulative symbol frequencies */
int ArithmeticCoder::decode_symbol ( int cum_freq[] )
{
   long range ; /* Size of current code region */
   int cum;    /* Cumulative frequency calculated */
   int symbol; /* Symbol decoded */

   range = (long)(high-low)+1;

   /* Flnd cum freq for _decoding_value. */
   cum = (((long)(_decoding_value-low)+1)*cum_freq[0]-1) / range ;

   for( symbol = 1; cum_freq[symbol]>cum ; symbol++) ; /* Then find symbol. */

   /* Narrow the code region to that allotted to this symbol. */
   high = low + (range*cum_freq[symbol-1])/cum_freq[0]-1 ;
   low = low + (range*cum_freq[symbol])/cum_freq[0] ;


   for( ;; )/* loop to gOt rid of bits. */
   {
      if ( high<Half )/* Expand low Half. */
      {
         /* nothing */
      }
      else if (low>=Half)/* Expand high Half. */
      {
         _decoding_value -= Half;/* Subtract offset to top. */
         low -= Half;
         high -= Half;
      }
      else if (low>=First_qtr && high<Third_qtr) /* Expand middle Half. */
      {
         _decoding_value -= First_qtr;/* Subtract offset to mIddlr*/
         low -= First_qtr;
         high -= First_qtr;
      }
      else break;/* Otherwise exit loop. */

      low = 2*low;
      high = 2*high+1;/* Scale up code range. */

      _decoding_value = 2*_decoding_value+_input.input_bit();/* Move in next input blt. */
   }
   return symbol;
}
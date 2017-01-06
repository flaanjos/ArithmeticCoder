/* THE ADAPTIVE SOURCE MODEL */

#include "AdaptativeModel.h"



/* INITIALIZE THE MODEL. */

int *AdaptativeModel::getCumulativeFrequency( )
{
   return _cumulative_frequencies ;
}

int AdaptativeModel::getSymbol( unsigned int ch )
{
   return _char_to_index[ ch ] ;
}


unsigned char AdaptativeModel::getChar( int symbol ) 
{
   return _index_to_char[symbol] ;
}

void AdaptativeModel::start()
{
   int i = 0 ;

   for( i=0 ; i<No_of_chars ; i++ )
   {
      // Set up tables that translate between symbol indexes and characters.
      _char_to_index[i] =  i+1 ;
      _index_to_char[i+1] = i ;
   }

   for( i=0 ; i<=No_of_symbols ; i++ )
   {
      // Set up initial frequency counts to be one for all symbols.
      _frequencies[i] = 1 ;                  
      _cumulative_frequencies[i] = No_of_symbols - i ;
   }
   // Freq[O] must not be the same as freq[l].
   _frequencies[0] = 0 ;
}


/* UPDATE THE MODEL TO ACCOUNT FOR A NEW SYMBOL. */
void AdaptativeModel::update( int symbol )
{
   int i ; /* New index for symbol */

   if (_cumulative_frequencies[0] == Max_frequency) /* See if frequency count */
   {
      for( i=No_of_symbols ; i>0 ; i-- )
      {
         _frequencies[i] = _cumulative_frequencies[i-1] - _cumulative_frequencies[i] ;
      }
    //  _frequencies[No_of_symbols] = cum_freq[No_of_symbols] ;

      for ( i=No_of_symbols ; i>0 ; i-- ) /* If so, halve all the */
      {
         _frequencies[i] = (_frequencies[i]+1)/2 ; /* counts (keeplng them */
      }

      _cumulative_frequencies[No_of_symbols] =  0 ;

      for ( i = No_of_symbols; i>0; i-- )  /* Set up cumulative*/
      {
         _cumulative_frequencies[i-1] = _cumulative_frequencies[i] + _frequencies[i] ; /* frequency counts.*/
      }
   }

   i = symbol-1 ;
   while (i>=0)
   {
      /* Increment the frequency  */
      /* count for the symbol and */
      _cumulative_frequencies[i] += 1 ;/* frequencies. */
      i-- ;
   }
}

//{
//   int i ; /* New index for symbol */
//
//   if (cum_freq[0] == Max_frequency) /* See if frequency count8 */
//   {
//      int cum; /* are at thslr maxlawn. */
//      cum = 0;
//
//      for ( i = No_of_symbols ; i>=0 ; i-- ) /* If so, halve all the */
//      {
//         freq[i] = (freq[i]+1)/2 ; /* counts (keeplng them */
//         cum_freq[i] = cum; /* non_zero). */
//         cum += freq[i];
//      }
//   }
//
//   for (i = symbol; freq[i]==freq[i-1]; i--) ; /* Find symbol's new Index. */
//
//   if (i<symbol) 
//   {
//      int ch_i, ch_symbol ;
//
//      ch_i = index_to_char[i]; /* Update the translation */
//      ch_symbol = index_to_char[symbol]; /* tables lf the symbol has */
//      index_to_char[i] = ch_symbol ; /* moved. */
//      index_to_char[symbol] = ch_i ;
//      char_to_index[ch_i] = symbol ;
//      char_to_index[ch_symbol] = 1 ;
//   }
//   freq[i] += 1;
//
//   while (i>0)/* Increment the frequency */
//   {/* count for the symbol and */
//      i -= 1;/* update the cumulative */
//      cum_freq[i] += 1 ;/* frequencies. */
//   }
//}

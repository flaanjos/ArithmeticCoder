/* MAIN PROGRAM FOR ENCODING. */
#include <stdio.h>
#include <stdlib.h>
//--
#include "ArithmeticEncoder.h"
#include "string.h"


static char* test_string = "Secando na Tailandia. Depois de enfrentar onibus, trem, 11horas de voo duas conexões, com direito a parada no oriente medio, taxi, 4 horas de catamara e conexao pra barquinho... finalmente chego em Ko Phi Phi, na ilha onde mora meu amigo. Sabe como é... o cara é de duque de caxias... e nao perdeu essa mania de morar longe!Secando na Tailandia. Depois de enfrentar onibus, trem, 11horas de voo duas conexões, com direito a parada no oriente medio, taxi, 4 horas de catamara e conexao pra barquinho... finalmente chego em Ko Phi Phi, na ilha onde mora meu amigo. Sabe como é... o cara é de duque de caxias... e nao perdeu essa mania de morar longe!Secando na Tailandia. Depois de enfrentar onibus, trem, 11horas de voo duas conexões, com direito a parada no oriente medio, taxi, 4 horas de catamara e conexao pra barquinho... finalmente chego em Ko Phi Phi, na ilha onde mora meu amigo. Sabe como é... o cara é de duque de caxias... e nao perdeu essa mania de morar longe!Secando na Tailandia. Depois de enfrentar onibus, trem, 11horas de voo duas conexões, com direito a parada no oriente medio, taxi, 4 horas de catamara e conexao pra barquinho... finalmente chego em Ko Phi Phi, na ilha onde mora meu amigo. Sabe como é... o cara é de duque de caxias... e nao perdeu essa mania de morar longe!";

void main( int argc, char *argv[] )
{
   unsigned char* input = (unsigned char* )test_string;
   int n_in = strlen( test_string ) + 1 ;

   unsigned char* compressed = new unsigned char[ n_in*2 ];
   unsigned char* decompressed = new unsigned char[ n_in*2 ];
   int buffer_size;

   ArithmeticEncoder coder;
   int n_comp = coder.encode( input, n_in, compressed, buffer_size );
   int n_decomp = coder.decode( compressed, n_comp, decompressed, buffer_size );

   printf( "%s\n\n", decompressed );

}

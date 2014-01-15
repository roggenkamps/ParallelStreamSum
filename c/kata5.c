#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAXVALS 10

struct state_data {
  uint64_t icount;
  uint8_t  values[MAXVALS];
  uint8_t  digits[MAXVALS];
  uint8_t  nactive;
};

int analyzeDigits( uint8_t digit, struct state_data *sdata )
{
  int index = sdata->icount % MAXVALS;
  int startIndex = index;
  int i;
  char out[10];
  uint8_t maxloop = (sdata->nactive < MAXVALS) ? sdata->nactive : MAXVALS;
  sdata->digits[index] = digit;
  sdata->values[index] = digit;
  for ( i = 0; i < maxloop; i++ ) {
    /*    int lindex = index+i+1; */
    int lindex = index-maxloop+i;
    if ( lindex < 0 ) {
      lindex = lindex + MAXVALS;
    }
    if ( sdata->values[lindex] == digit ) {
      int v = digit;
      int j = lindex;
      int k = 0;
      while ( v > 0 ) {
	out[k]=sdata->digits[j]+'0';
	j++;
	k++;
	if ( j >= MAXVALS ) { j += MAXVALS; }
	v -= sdata->digits[j];
      }
      out[k] =0;
      printf( "%s\n", out );
    }
    if ( sdata->values[lindex] != 255 ) {
      sdata->values[lindex] += digit;
      if ( sdata->values[lindex] > 9 ) {
	sdata->values[lindex] = 255;
	sdata->nactive--;
      }
    }
  }
  
  sdata->icount++;
  if ( sdata->nactive < MAXVALS ) { sdata->nactive++; }
  return 1;
}

int main( int argc, char **argv )
{
  static char instr[] = "8745648184845171326578518184151512461752149647129746915414816354846454";
    /* static char instr[] = "24617"; */
  uint8_t  digits[sizeof(instr)];
  uint64_t count = 0;
  uint64_t digitIndex = 0;
  uint64_t inLength = sizeof( instr )-1;
  struct state_data  *sdata;
  long   i;
  
  if ( argc != 2 ) {
    fprintf( stderr, "usage:  kata5 count\n" );
  }

  count = strtol( *(argv+1), NULL, 0 );

  /* allocate memory */
  sdata = (struct state_data *) mmap(NULL,
				     sizeof(struct state_data),
				     PROT_READ|PROT_WRITE,
				     MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED | MAP_POPULATE,
				     -1, 0
				     );

  if ( sdata == MAP_FAILED ) {
    perror( "map failed" );
    exit( EXIT_FAILURE );
  }

  for ( i = 0; i < inLength; i++ ) {
    digits[i] = instr[i] - '0';
  }

  for ( i = 0; i < count; i++ ) {
    digitIndex = i % inLength;
    if ( analyzeDigits( digits[digitIndex], sdata ) == -1 ) {
      exit( EXIT_FAILURE );
    }
  }

  exit( EXIT_SUCCESS );
}


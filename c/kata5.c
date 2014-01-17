#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAXVALS 10

struct state_data {
  uint64_t icount;
  uint64_t  values;
  uint64_t  digits;
  uint64_t  multiplier;
  int       nactive;
  char      outbuf[4096];
  int       outsz;
};

int val( uint64_t x, int i ) {
  return (( x & ((uint64_t)0x1f << (i*5))) >> (i*5));
}

int analyzeDigits( uint8_t digit, struct state_data *sdata )
{
  static const uint64_t masks[] = { ((uint64_t) 0x1f) << 0,
				    ((uint64_t) 0x1f) << 5,
				    ((uint64_t) 0x1f) <<10,
				    ((uint64_t) 0x1f) <<15,
				    ((uint64_t) 0x1f) <<20,
				    ((uint64_t) 0x1f) <<25,
				    ((uint64_t) 0x1f) <<30,
				    ((uint64_t) 0x1f) <<35,
				    ((uint64_t) 0x1f) <<40,
				    ((uint64_t) 0x1f) <<45
  };
  int      v    = 0;
  int      out  = 0;
  uint64_t mask = 0;
  int      i;
  sdata->multiplier = (sdata->multiplier << 5) + 1;
  uint64_t delta = sdata->multiplier * digit;
  sdata->values  <<= 5;
  sdata->digits  = (sdata->digits << 5) + digit;
  uint64_t delv = sdata->values ^ delta;
  for (i=sdata->nactive; i > 1; i-- ) {
    uint64_t m  = masks[i];
    /* uint64_t x= delv & m; */
    if ( (uint64_t)(delv & m) == (uint64_t) 0 ) {
      int      ov = (sdata->values & m) >> (i*5);
      int      n  = i;
      while ( ov >= 0 ) {
	int d = (sdata->digits & m) >> (n--*5);
	ov -= d;
	sdata->outbuf[sdata->outsz++] = d + '0';
	m >>= 5;
      }
      sdata->outbuf[sdata->outsz++] = '\n';
      if ( sdata->outsz > sizeof( sdata->outbuf ) - 32 ) {
	fwrite( sdata->outbuf, sdata->outsz, 1, stdout );
	sdata->outsz = 0;
      }
    }
  }
  sdata->values += delta;
  while ((v = (sdata->values & masks[sdata->nactive]) >> (sdata->nactive*5)) > 9 ) {
    sdata->values = sdata->values ^ (v << (sdata->nactive*5));
    sdata->digits = sdata->digits ^ (sdata->digits & mask);
    sdata->multiplier >>= 5;
    sdata->nactive--;
  }
  sdata->nactive++;
  sdata->icount++;
  return 1;
}

int main( int argc, char **argv )
{
  /*                                    |--|        |-|   |-|-|                                */
  static char instr[] = "8745648184845171326578518184151512461752149647129746915414816354846454";
  /*  static char instr[] = "111111111911248124617";
      static char instr[] = "24617";*/
  uint8_t  digits[sizeof(instr)];
  uint64_t count = 0;
  uint64_t digitIndex = 0;
  uint64_t inLength = sizeof( instr )-1;
  struct state_data  sdata;
  long   i;
  
  if ( argc != 2 ) {
    fprintf( stderr, "usage:  kata5 count\n" );
  }

  count = strtol( *(argv+1), NULL, 0 );

  /* allocate memory */
  for ( i = 0; i < inLength; i++ ) {
    digits[i] = instr[i] - '0';
  }

  for ( i = 0; i < count; i++ ) {
    digitIndex = i % inLength;
    if ( analyzeDigits( digits[digitIndex], &sdata ) == -1 ) {
      exit( EXIT_FAILURE );
    }
  }

  if ( sdata.outsz > 0 ) {
    fwrite( sdata.outbuf, sdata.outsz, 1, stdout );
  }

  exit( EXIT_SUCCESS );
}


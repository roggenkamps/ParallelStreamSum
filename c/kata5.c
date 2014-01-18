#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAXVALS 10

static struct state_data {
  uint64_t icount;
  uint64_t  values;
  uint64_t  digits;
  uint64_t  multiplier;
  int       nactive;
  char      outbuf[4096];
  int       outsz;
} state_data;

int val( uint64_t x, int i ) {
  return (( x & ((uint64_t)0x1f << (i*5))) >> (i*5));
}

int analyzeDigits( uint8_t digit, struct state_data *sdata )
{
  int      v    = 0;
  int      out  = 0;
  uint64_t mask = 0;
  int      i;
  sdata->multiplier = (sdata->multiplier << 5) + 1;
  uint64_t delta    = sdata->multiplier * digit;
  sdata->values     <<= 5;
  sdata->digits     = (sdata->digits << 5) + digit;
  
  for (i=sdata->nactive; i > 1; i-- ) {
    if (((v = (sdata->values & (mask=(((uint64_t)0x1f) << (i*5)))) >> (i*5)) == digit )) {
      int      n  = i;
      while ( v >= 0 ) {
	int d = (sdata->digits & mask) >> (n*5);
	mask >>= 5;
	n--;
	sdata->outbuf[sdata->outsz++] = d + '0';
	v -= d;
      }
      sdata->outbuf[sdata->outsz++] = '\n';
      if ( sdata->outsz > sizeof( sdata->outbuf ) - 32 ) {
	fwrite( sdata->outbuf, sdata->outsz, 1, stdout );
	sdata->outsz = 0;
      }
    }
  }
  sdata->values += delta;
  while ((v = (sdata->values & (mask=(((uint64_t)0x1f) << (sdata->nactive*5)))) >> (sdata->nactive*5)) > 9 ) {
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
  /* static char instr[] = "111111111911248124617"; */
  uint8_t  digits[sizeof(instr)];
  uint64_t count = 0;
  uint64_t inLength = sizeof( instr )-1;
  struct state_data  *sdata = &state_data;
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
    uint64_t digitIndex = i % inLength;
    uint8_t digit = digits[digitIndex];
    int      v    = 0;
    int      out  = 0;
    uint64_t mask = 0;
    int      i;
    sdata->multiplier = (sdata->multiplier << 5) + 1;
    uint64_t delta    = sdata->multiplier * digit;
    sdata->values     <<= 5;
    sdata->digits     = (sdata->digits << 5) + digit;
  
    for (i=sdata->nactive; i > 1; i-- ) {
      if (((v = (sdata->values & (mask=(((uint64_t)0x1f) << ((i<<2)+i)))) >> ((i<<2)+i)) == digit )) {
	int      n  = i;
	while ( v >= 0 ) {
	  int d = (sdata->digits & mask) >> ((n<<2)+n);
	  mask >>= 5;
	  n--;
	  sdata->outbuf[sdata->outsz++] = d + '0';
	  v -= d;
	}
	sdata->outbuf[sdata->outsz++] = '\n';
	if ( sdata->outsz > sizeof( sdata->outbuf ) - 32 ) {
	  fwrite( sdata->outbuf, sdata->outsz, 1, stdout );
	  sdata->outsz = 0;
	}
      }
    }
    sdata->values += delta;
    while ((v = (sdata->values & (mask=(((uint64_t)0x1f) << (sdata->nactive*5)))) >> (sdata->nactive*5)) > 9 ) {
      sdata->values ^= (v << (sdata->nactive*5));
      sdata->digits ^= (sdata->digits & mask);
      sdata->multiplier >>= 5;
      sdata->nactive--;
    }
    sdata->nactive++;
    sdata->icount++;
  }

  if ( sdata->outsz > 0 ) {
    fwrite( sdata->outbuf, sdata->outsz, 1, stdout );
  }

  exit( EXIT_SUCCESS );
}


#include <stdio.h>
#include <stdlib.h>

typedef struct Candle{
	float open;
	float min;
	float max;
	float close;
	int date_open;
	int date_close;
	unsigned char point;
	float volume;
	float value;
} Candle;


void Gap(int candles_count, struct Candle *candles, char *values)
{
	values[0] = 0;
    for(int i=1; i<candles_count; i++) {
		if(candles[i-1].max < candles[i].min) {
			values[i] = 1;
		}
		else if(candles[i-1].min > candles[i].max) {
			values[i] = -1;
		}
		else {
			values[i] = 0;
		}
    }
}

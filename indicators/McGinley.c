#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


void mg(int candles_count, Candle* candles, float* res_values)
{
    res_values[0] = candles[0].close;
    for(int i=1; i<candles_count; i++) {
        res_values[i] = res_values[i-1] + (candles[i].close - res_values[i-1]) / ( 14 * pow( (candles[i].close / res_values[i-1]), 4 ) );
    }
}
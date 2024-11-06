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


void ThrustDay(int candles_count, struct Candle *candles, float p)
{
    for(int i=1; i<candles_count; i++) {
		if(candles[i].close - candles[i-1].max > p) {
			printf("%d: ThrustDayHigh\n", i);
		}
		else if(candles[i-1].min - candles[i].close > p) {
			printf("%d: ThrustDayLow\n", i);
		}
    }
}
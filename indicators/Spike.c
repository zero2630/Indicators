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


float max_candle(struct Candle *candles, size_t left_end, size_t right_end)
{
    float max = candles[left_end].max;
    for(int i=0; left_end+i<right_end; i++) {
        if(candles[left_end+i].max>max) max = candles[left_end+i].max;
    }
    return max;
}


float min_candle(struct Candle *candles, size_t left_end, size_t right_end)
{
    float min = candles[left_end].min;
    for(int i=0; left_end+i<right_end; i++) {
        if(candles[left_end+i].min<min) min = candles[left_end+i].min;
    }
    return min;
}



void Spike(int candles_count, struct Candle *candles, float p, int n)
{
    for(int i=1; i<candles_count; i++) {
		if(candles[i].max - fmax(max_candle(candles, i-n, i), max_candle(candles, i+1, i+n+1)) > p) {
			printf("%d: SpikeMax\n", i);
		}
		else if(fmin(min_candle(candles, i-n, i), min_candle(candles, i+1, i+n+1)) - candles[i].min > p) {
			printf("%d: SpikeMin\n", i);
		}
    }
}
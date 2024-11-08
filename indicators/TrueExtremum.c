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
    for(int i=left_end; i<right_end; i++) {
        if(candles[left_end+i].max>max) max = candles[left_end+i].max;
    }
    return max;
}


float min_candle(struct Candle *candles, size_t left_end, size_t right_end)
{
    float min = candles[left_end].min;
    for(int i=left_end; i<right_end; i++) {
        if(candles[left_end+i].min<min) min = candles[left_end+i].min;
    }
    return min;
}


void true_extremum(int length, Candle* candles, int n, double* res_values_max, double* res_values_min)
{
    float max, min;
    
    for(int i=n; i<length-n-1; i++) {
        max = max_candle(candles, i-n, i+n+1);
        min = min_candle(candles, i-n, i+n+1);

        if(max > candles[i-1].close) res_values_max[i] = max;
        else res_values_max[i] = candles[i-1].close;

        if(min < candles[i-1].close) res_values_min[i] = min;
        else res_values_min[i] = candles[i-1].close;
    }
}
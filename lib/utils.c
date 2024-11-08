#include "utils.h"

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
#include <stdlib.h>

#include "utils.h"

void SMA(int length, float *values, float *SMA_values, int wide)
{
    SMA_values[0] = values[0];

    for(int i=1; i<length; i++)
    {
        if(i+1<=wide)
        {
            float el_sum = 0;
            for(int j=0; j<i+1; j++)
            {
                el_sum += values[j];
            }
            SMA_values[i] = el_sum / (i+1);
        }
        else
        {
            SMA_values[i] = SMA_values[i-1] - values[i-wide] / wide + values[i] / wide;
        }
    }

}

void true_extremum(int length, Candle* candles, int n, float* res_values_max, float* res_values_min)
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

void super_trend(int length, Candle* candles, int n, float multiplier, char *trend)
{
    float *atr = calloc(length, sizeof(float));
    float *truemax = calloc(length, sizeof(float));
    float *truemin = calloc(length, sizeof(float));
    float *substract_res = calloc(length, sizeof(float));
    float *src = calloc(length, sizeof(float));
    float *p = calloc(length, sizeof(float));
    float *up = calloc(length, sizeof(float));
    float *up1 = calloc(length, sizeof(float));
    float *dn = calloc(length, sizeof(float));
    float *dn1 = calloc(length, sizeof(float));

    true_extremum(length, candles, n, truemax, truemin);
    for(int i=0; i<length; i++) {
        substract_res[i] = truemax[i] - truemin[i];
    }
    SMA(length, substract_res, atr, n);

    for(int i=1; i<length; i++) {
        src[i] = (candles[i].max-candles[i].min)/2;
        up[i] = src[i] - atr[i] * multiplier;

        if(up[i-1] != 0) up1[i] = up[i-1];
        else if(up[i] != 0) up1[i] = up[i];

        if(candles[i-1].close > up[i]) up[i] = fmaxf(up[i], up1[i]);

        dn[i] = src[i] + atr[i] * multiplier;

        if(dn[i-1] != 0) dn1[i] = dn[i] = dn[i-1];
        else if(dn[i] != 0) dn1[i] = dn[i];

        if(candles[i].close < dn1[i]) dn[i] = fminf(dn[i], dn1[i]);

        trend[i] = 1;
        if(trend[i-1] != 0) trend[i] = trend[i-1];

        if(candles[i].close > dn1[i]) trend[i] = 1;
        else if(candles[i].close < up1[i]) trend[i] = -1;

    }
}
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




void SMA(int length, float *values, float *SMA_values, unsigned int wide)
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


void EMA(int length, float *values, float *EMA_values, unsigned int wide)
{
    EMA_values[0] = values[0];
    const float k = (float)2 / (float)(wide + 1);

    for(int i=1; i<length; i++)
    {
            EMA_values[i] = values[i] * k + EMA_values[i-1] * (1 - k);
    }

}

float calculateStandardDeviation(int N, int start_pos, float *values, float *SMA_values)
{
 
    float quadrats = 0;
 
    for (int i = start_pos; i < N; i++) {
        quadrats += pow(values[i] - SMA_values[i], 2);
    }
 
    float variance = quadrats / N;
 
    float standardDeviation = sqrt(variance);
 
    return standardDeviation;
}

void BollingerBands(int candles_count, struct Candle *candles, float *upperBB, float *lowerBB, size_t BB_length, size_t BB_mult)
{
    float *close_values = malloc(candles_count * sizeof(float));
    float *basic = malloc(candles_count * sizeof(float));
    float deviation;

    for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;

    SMA(candles_count, close_values, basic, BB_length);
    
    for(int i=BB_length; i<candles_count; i++) {
        deviation = BB_mult * calculateStandardDeviation(i+1, i-BB_length+1, close_values, basic);
        upperBB[i] = basic[i] + deviation;
        lowerBB[i] = basic[i] - deviation;
    }

    free(close_values);
    free(basic);
}

void KeltnerChannel(int candles_count, struct Candle *candles, float *upperKC, float *lowerKC, size_t KC_length, size_t KC_mult, char useTrueRange)
{
    float *close_values = malloc(candles_count * sizeof(float));
    float *ma = malloc(candles_count * sizeof(float));
    float *rangema = malloc(candles_count * sizeof(float));
    float *range = calloc(candles_count, sizeof(float));

    for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;

    SMA(candles_count, close_values, ma, KC_length);

    {
        float high, low, prev_close;
        if(useTrueRange) {
            for(int i=1; i<candles_count; i++) {
                high = candles[i].max;
                low = candles[i].min;
                prev_close = candles[i-1].close;
                range[i] = fmax(fmax(high - low, fabs(high - prev_close)), fabs(low - prev_close));
            }
        }
        else {
            for(int i=1; i<candles_count; i++) {
                high = candles[i].max;
                low = candles[i].min;
                range[i] = high - low;
            }
        }

    }

    SMA(candles_count, range, rangema, KC_length);

    for(int i=KC_length; i<candles_count; i++) {
        upperKC[i] = ma[i] + rangema[i] * KC_mult;
        lowerKC[i] = ma[i] - rangema[i] * KC_mult;
    }

    free(close_values);
    free(ma);
    free(rangema);
    free(range);

}


void SqueezeMomentum(int candles_count, struct Candle *candles, size_t BB_length, size_t BB_mult, size_t KC_length, size_t KC_mult, char useTrueRange)
{
    float *upperBB = calloc(candles_count, sizeof(float));
    float *lowerBB = calloc(candles_count, sizeof(float));
    float *upperKC = calloc(candles_count, sizeof(float));
    float *lowerKC = calloc(candles_count, sizeof(float));

    BollingerBands(candles_count, candles, upperBB, lowerBB, BB_length, BB_mult);
    KeltnerChannel(candles_count, candles, upperKC, lowerKC, KC_length, KC_mult, useTrueRange);

    for(int i=0; i<candles_count; i++) {
        if(lowerBB[i] > lowerKC[i] && upperBB[i] < upperKC[i]) printf("squeeze on\n");
        else if(lowerBB[i] < lowerKC[i] && upperBB[i] > upperKC[i]) printf("squeeze released\n");
        else printf("no squeeze");
    }
}
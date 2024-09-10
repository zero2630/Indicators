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


double calculateStandardDeviation(int N, int start_pos, double *values)
{
    
    double sum = 0;
    for (int i = start_pos; i < N; i++) {
        sum += values[i];
    }
 
    double middle_val = sum / N;
 
    double quadrats = 0;
 
    for (int i = start_pos; i < N; i++) {
        quadrats += pow(values[i] - middle_val, 2);
    }
 
    double variance = quadrats / N;
 
    double standardDeviation = sqrt(variance);
 
    return standardDeviation;
}


void SMA(int length, double *values, double *SMA_values, unsigned int wide)
{
    SMA_values[0] = values[0];

    for(int i=1; i<length; i++)
    {
        if(i+1<=wide)
        {
            double el_sum = 0;
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


void EMA(int length, double *values, double *EMA_values, unsigned int wide)
{
    EMA_values[0] = values[0];
    const double k = (double)2 / (double)(wide + 1);

    for(int i=1; i<length; i++)
    {
            EMA_values[i] = values[i] * k + EMA_values[i-1] * (1 - k);
    }

}

void BollingerBands(int candles_count, struct Candle *candles, size_t BB_length, size_t BB_mult)
{
    double *close_values = malloc(candles_count * sizeof(double));
    double *basic = malloc(candles_count * sizeof(double));
    double deviation;
    double *upperBB = calloc(candles_count, sizeof(double)), *lowerBB = calloc(candles_count, sizeof(double));

    for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;

    SMA(candles_count, close_values, basic, BB_length);
    
    for(int i=BB_length; i<candles_count; i++) {
        deviation = BB_mult * calculateStandardDeviation(i+1, i-BB_length+1, close_values);
        upperBB[i] = basic[i] + deviation;
        lowerBB[i] = basic[i] - deviation;
    }

    
    printf("upperBB: %lf | lowerBB: %lf\n", upperBB[i], lowerBB[i]);
}
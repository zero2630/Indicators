
void PVI(int candles_count, struct Candle *candles)
{
    float *pvi_array = calloc(candles_count, sizeof(float));

    for(int i=1; i<candles_count; i++)
    {
        if(candles[i].volume > candles[i-1].volume && candles[i].close > candles[i-1].close)
        {
            pvi_array[i] = pvi_array[i-1] + (candles[i].close - candles[i-1].close) / candles[i-1].close;
        }
    }
    for(int i=0; i<candles_count; i++)
    {
        printf("%lf\n", pvi_array[i]);
    }
}
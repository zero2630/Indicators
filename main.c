#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #include "utils.h"


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

int save_bin(int candles_count, Candle *candles)
{
	FILE *bin;
	char *p;
	char *filename = calloc(30, sizeof(char));

	printf("Enter name of binary file to save candles: ");
	strcat(filename, "binary/");
	scanf("%s", filename + 7);
	printf("path: %s\r\n", filename);

	bin = fopen(filename, "wb");
	if (!bin) {
		printf("Error while opening binary file to write data\r\n");
		return 1;
	}
	
	printf("%d\r\n", candles_count);
	fwrite(&candles_count, sizeof(int), 1, bin);

	unsigned int size = (unsigned int)sizeof(Candle);
	for (int i = 0; i < candles_count; i++) {
		printf("%d\r\n", i);
		p = (char *)(candles + i);
		for (unsigned int j = 0; j < size; j++) {
			putc(*p++, bin);
		}
	}
	fclose(bin);
	free(filename);
	return 0;
}

int load_bin(int *candles_count, Candle **candles, int data_frame)
{
	FILE *bin;
	char *p;
	char *filename = calloc(30, sizeof(char));

	// printf("Enter name of binary file to load candles: ");
	sprintf(filename, "binary/gazp_%02d.bin", data_frame);
	//scanf("%s", filename + 7);
	printf("path: %s\r\n", filename);

	bin = fopen(filename, "rb");
	if (!bin) {
		printf("Error while opening binary file to load data\r\n");
		return 1;
	}

	fread(candles_count, sizeof(int), 1, bin);

	size_t size = sizeof(Candle);
	*candles = calloc(*candles_count, sizeof(Candle));

	for (int i = 0; i < *candles_count; i++) {
		p = (char *)(*candles + i);
		for (size_t j = 0; j < size; j++) {
			*p = getc(bin);
			p++;
		}
	}
	fclose(bin);
	free(filename);
	return 0;
}

int load_text(int *candles_count, Candle **candles)
{
	FILE *f;
    char *filename = calloc(30, sizeof(char));
    sprintf(filename, "./data/data.txt");

    f = fopen(filename, "r");
    if (!f) {
        printf("Error while opening text file to load data\r\n");
        return 1;
    }

    *candles_count = 10000;
	*candles = calloc(*candles_count, sizeof(Candle));

	for (int i = 0; i < *candles_count; i++) {
		// values order in string:
		// date_open, date_close, open, close, max, min, volume, value
		fscanf(f, "%d %d %f %f %f %f %f %f", &((*candles)[i]).date_open, &((*candles)[i]).date_close,
											 &((*candles)[i]).open, &((*candles)[i]).close, &((*candles)[i]).max,
											 &((*candles)[i]).min, &((*candles)[i]).volume, &((*candles)[i]).value);
	}
	fclose(f);
	return 0;
}

void buysell(Candle *candles, int candles_count, float start_cash, int *arr)
{
	FILE *f = fopen("buysell.txt", "w");

	float a = start_cash;
	int cnt = 0;
	int cnt1 = 0, cnt2 = 0;
	int p;
	int count_operat = 0;
	int count_operat1 = 0;
	int count_operat2 = 0;
	
	float max_val = 0.0;

	for (int i = 0; i < candles_count; i++)
	{
		p = floor(a/candles[i].max);
		if (arr[i] == 1 && (a - p * candles[i].max> 0)) {
			cnt1++;
			a -= candles[i].max*p;
			cnt+=p;
			fprintf(f,"BUY: %f %d %f %s\r\n", candles[i].max, cnt, a, ctime((const time_t *)&candles[i].date_close));
			count_operat++;
			count_operat1++;
		}
		if (arr[i] == 2 && cnt > 0) {
			cnt2++;
			a += candles[i].min*cnt;
			cnt=0;
			fprintf(f,"SELL: %f %d %f %s\r\n", candles[i].max, cnt, a, ctime((const time_t *)&candles[i].date_close));
			count_operat++;
			count_operat2++;
		}
		if (a > max_val)
			max_val = a;
	}

	if (cnt) {
		cnt2++;
		a += candles[candles_count-1].min*cnt;
		cnt=0;
		fprintf(f,"SELL: %d %f %s\r\n", cnt, a, ctime((const time_t *)&candles[candles_count - 1].date_close));
		count_operat++;
		count_operat2++;
	}

	fprintf(f,"%d %d %f %d %d %d\r\n", cnt1, cnt2, a, count_operat, count_operat1, count_operat2);
	fclose(f);
	printf("max value is %f\r\n", max_val);
}


void save_indicator_text(int length, char *values)
{
	FILE *f = fopen("./data/indicator_data.txt", "w");
	for(int i=0; i<length; i++)
	{
		fprintf(f, "%hhd\n", values[i]);
	}
	fclose(f);
}


void save_lines_text(int length, float *values)
{
	FILE *f = fopen("./data/lines_data.txt", "w");
	for(int i=0; i<length; i++)
	{
		fprintf(f, "%lf\n", values[i]);
	}
	fclose(f);
}


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


void ThrustDay(int candles_count, struct Candle *candles, float p, char *values)
{
	values[0] = 0;
    for(int i=1; i<candles_count; i++) {
		if(candles[i].close - candles[i-1].max > p) {
			values[i] = 1;
		}
		else if(candles[i-1].min - candles[i].close > p) {
			values[i] = -1;
		}
		else {
			values[i] = 0;
		}
    }
}


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



void Spike(int candles_count, struct Candle *candles, float p, int n, char *values)
{
	for(int i=0; i<n; i++) {
		values[i] = 0;
	}
    for(int i=n; i<candles_count; i++) {
		if(candles[i].max - fmaxf(max_candle(candles, i-n, i), max_candle(candles, i+1, i+n+1)) > p) {
			values[i] = 1;
		}
		else if(fminf(min_candle(candles, i-n, i), min_candle(candles, i+1, i+n+1)) - candles[i].min > p) {
			values[i] = -1;
		}
		else {
			values[i] = 0;
		}
    }
}


void PVI(int candles_count, struct Candle *candles, float* values)
{
    for(int i=1; i<candles_count; i++)
    {
        if(candles[i].volume > candles[i-1].volume && candles[i].close > candles[i-1].close)
        {
            values[i] = values[i-1] + (candles[i].close - candles[i-1].close) / candles[i-1].close;
        }
    }
}

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
        if(trend[i-1] != 0) {
			trend[i] = trend[i-1];
		}

        if(candles[i].close > dn1[i]) {
			trend[i] = 1;
		}
        else if(candles[i].close < up1[i]) {
			trend[i] = -1;
		}
		printf("%f %f\n", candles[i].close, dn1[i]);

    }
}


int main(int argc, char* argv[]) {
    // gcc main.c -Wall -Wextra -o  main -lm
	/*
		01 - минутные свечки
		02 - 5-минутные свечки
		03 - 15-минутные свечки
		04 - 30-минутные свечки
		05 - часовые свечки
		06 - двухчасовые свечки
		07 - четырёхчасовые свечки
	*/
	int candles_count;
	struct Candle *candles = NULL;
    // Выбор отрезка данных
	// int frame = 5; //atoi(argv[1]);

    // Получение свечей из бинарника
	// load_bin(&candles_count, &candles, frame);
	load_text(&candles_count, &candles);

    /* начало логики работы со свечами */
	char *indicator_values = calloc(candles_count, sizeof(char));
	float *lines_values = calloc(candles_count, sizeof(float));
	float *lines_values2 = calloc(candles_count, sizeof(float));

	super_trend(candles_count, candles, 10, 2, indicator_values);
	save_indicator_text(candles_count, indicator_values);

	if(argc==3) {
		if(strcmp(argv[1], "i") == 0) {
			if(strcmp(argv[2], "spike") == 0) {
				Spike(candles_count, candles, 1, 10, indicator_values);
			}
			else if(strcmp(argv[2], "gap") == 0) {
				Gap(candles_count, candles, indicator_values);
			}
			else if(strcmp(argv[2], "thrust_day") == 0) {
				ThrustDay(candles_count, candles, 1, indicator_values);
			}
			else if(strcmp(argv[2], "super_trend") == 0) {
				super_trend(candles_count, candles, 10, 2, indicator_values);
			}
			save_indicator_text(candles_count, indicator_values);
		}

		else if(strcmp(argv[1], "l") == 0) {
			if(strcmp(argv[2], "pvi") == 0) {
				PVI(candles_count, candles, lines_values);
			}
			else if(strcmp(argv[2], "sma") == 0) {
				float *close_values = malloc(candles_count * sizeof(float));
				for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;
				SMA(candles_count, close_values, lines_values, 5);
			}
			else if(strcmp(argv[2], "truemax") == 0) {
				true_extremum(candles_count, candles, 10, lines_values, lines_values2);
			}
			save_lines_text(candles_count, lines_values);
		}
	}
    /* конец логики работы со свечами */

	return 0;
}

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>


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

int load_text(int *candles_count, Candle **candles, char *graphic_name, int data_frame)
{
	FILE *f;
    char *filename = calloc(30, sizeof(char));
    sprintf(filename, "text/%s_%02d.txt", graphic_name, data_frame);

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

void BollingerBands(int candles_count, struct Candle *candles, double *upperBB, double *lowerBB, size_t BB_length, size_t BB_mult)
{
    double *close_values = malloc(candles_count * sizeof(double));
    double *basic = malloc(candles_count * sizeof(double));
    double deviation;

    for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;

    SMA(candles_count, close_values, basic, BB_length);
    
    for(int i=BB_length; i<candles_count; i++) {
        deviation = BB_mult * calculateStandardDeviation(i+1, i-BB_length+1, close_values);
        upperBB[i] = basic[i] + deviation;
        lowerBB[i] = basic[i] - deviation;
    }

    free(close_values);
    free(basic);
}

void KeltnerChannel(int candles_count, struct Candle *candles, double *upperKC, double *lowerKC, size_t KC_length, size_t KC_mult, char useTrueRange)
{
    double *close_values = malloc(candles_count * sizeof(double));
    double *ma = malloc(candles_count * sizeof(double));
    double *rangema = malloc(candles_count * sizeof(double));
    double *range = calloc(candles_count, sizeof(double));

    for(int i=0; i<candles_count; i++) close_values[i] = candles[i].close;

    SMA(candles_count, close_values, ma, KC_length);

    {
        double high, low, prev_close;
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
    double *upperBB = calloc(candles_count, sizeof(double));
    double *lowerBB = calloc(candles_count, sizeof(double));
    double *upperKC = calloc(candles_count, sizeof(double));
    double *lowerKC = calloc(candles_count, sizeof(double));

    BollingerBands(candles_count, candles, upperBB, lowerBB, BB_length, BB_mult);
    KeltnerChannel(candles_count, candles, upperKC, lowerKC, KC_length, KC_mult, useTrueRange);

    for(int i=0; i<candles_count; i++) {
        if(lowerBB[i] > lowerKC[i] && upperBB[i] < upperKC[i]) printf("squeeze on\n");
        else if(lowerBB[i] < lowerKC[i] && upperBB[i] > upperKC[i]) printf("squeeze released\n");
        else printf("no squeeze\n");
    }

	free(upperBB);
	free(lowerBB);
	free(upperKC);
	free(lowerKC);
}


int main() {
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
	int frame = 5; //atoi(argv[1]);

    // Получение свечей из бинарника
	// load_bin(&candles_count, &candles, frame);
	load_text(&candles_count, &candles, "sber", frame);

    /* начало логики работы со свечами */
	
	//BollingerBands(candles_count, candles, 20, 2.0);
	//KeltnerChannel(candles_count, candles, upperKC, lowerKC, 20, 1.5, 1);
	SqueezeMomentum(candles_count, candles, 20, 2.0, 20, 1.5, 1);

    /* конец логики работы со свечами */

	return 0;
}

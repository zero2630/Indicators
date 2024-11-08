#ifndef UTILS_H_
#define UTILS_H_

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

float max_candle(struct Candle *candles, size_t left_end, size_t right_end);
float min_candle(struct Candle *candles, size_t left_end, size_t right_end);

#endif
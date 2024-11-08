#include "candle.h"

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
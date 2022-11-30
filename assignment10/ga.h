#pragma once
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>


#define seedval	( (unsigned)time( NULL ) )
#define TRUE	1
#define FALSE	0
#define GENES	4

#define POPULATION_SIZE	6
#define PCROSS			0.7 
#define PMUT			0.01//0.001
#define GA_END			100

#define drand()			((double)rand()/RAND_MAX) //[0.0..1.0] ������ ���� �߻�
#define random(i, n)	(rand()%((n)-(i))+(i))    //[i..n-1] ������ ���� �߻�

#define SWAP(x, y, t) ((t)=(x), (x)=(y), (y)=(t))

struct chromosome {
	int genes[GENES];
	int x;
	int fitness;
};

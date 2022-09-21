#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 10000

int list[MAX_SIZE];
int get_max(int low, int high) {
	int mid, max1, max2;
	//아래를 완성하시오.
	mid = (low + high) / 2;
	if (low == high)
	{
		return low;
	}
	else
	{
		max1 = get_max(low, mid);
		max2 = get_max(mid + 1, high);
		if (list[max1] >= list[max2])
		{
			return max1;
		}
		else
		{
			return max2;
		}
	}
}
int main(void)
{
	for (int i = 0; i < MAX_SIZE; i++)
		list[i] = rand() % 1000;
	int max = get_max(0, MAX_SIZE - 1);
	printf("최대값: %d\n", list[max]);

	return 0;
}
/*아래에 알고리즘의 분석 및 차수를 쓰시오.

	위의 get_max(int low, int high) 함수는 리스트의 가장 작은 인덱스와, 큰 인덱스를 입력받고 
	mid 값은 리스트의 중간 인덱스이다.
	최대값을 찾는 과정은 mid를 기준으로 왼쪽 배열에서 최대값과 오른쪽 배열에서의 최대값중 더 큰 값을 최대값으로 한다.

	수행시간을 T(n) 이라 하면 
	T(n) = 1 (n 이 1인경우)
	T(n) = 2T(n/2) + 1
	마스터 정리에 의해
	T(n) = Θ(n) 이다.


*/
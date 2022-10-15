#include <stdio.h>

int binsearch(int low, int high,int x, int S[]) {
	int mid;
	if (low > high) {
		return 0;
	}
	else                                        // 이분검색 순환호출 알고리즘
	{
		mid = (low + high) / 2;
		if (x==S[mid])
		{
			return mid;
		}
		else if (x < S[mid])
		{
			return binsearch(low, mid - 1, x, S);
		}
		else
		{
			return binsearch(mid + 1, high, x, S);
		}
	}
}
//
//void merge(int list[], int left, int mid, int right) {
//	int i, j, k, l;
//	i = left, j = mid + 1, k = left;
//
//	while (i<=mid && j<=right)
//	{
//		if (list[i]<=list[j])
//		{
//			
//		}
//	}
//}

void merge_sort(int list[], int left, int right) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		merge_sort(list, left, mid);
		merge_sort(list, mid + 1, right);
		//merge(list, left, mid, right);
	}
}

int main(void) {

	/*int arr[6] = { 1, 2, 3, 4, 5, 6 };

	int s =binsearch(0, 5, 2, arr);

	printf("%d", s);*/


}
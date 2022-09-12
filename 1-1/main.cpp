#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void prob_3() {
	int n = 5;
	int i, j, k;

	char str[] = "01234";
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			for (k = j + 1; k < n; k++)
				printf("%c%c%c, ", str[i], str[j], str[k]);


	printf("\n");
}
void prob_6() {
	int i, n = 10;
	int max, min;
	int arr[] = { 1, 5, 3, 7, 9, 8, 2, 4, 6, 0 };

	if (arr[0] < arr[1]) {
		min = arr[0]; max = arr[1];
	}
	else {
		min = arr[1]; max = arr[0];
	}
	for (i = 2; i <= n - 1; i += 2) {// �Է� 2���� 3�� ��
		if (arr[i] < arr[i + 1]) {
			if (arr[i] < min)
				min = arr[i];
			if (arr[i + 1] > max)
				max = arr[i + 1];
		}
		else {
			if (arr[i + 1] < min)
				min = arr[i + 1];
			if (arr[i] > max)
				max = arr[i];
		}
	}
	printf("min:%d, max:%d\n", min, max);
}
void prob_13() {
	int n, min_n, max_n;//min_n: n�� �ּҰ�, max_n: n�� �ִ밪
	min_n = max_n = 2;
	double A = 10.0 * max_n * max_n;
	double B = 300.0 * log2(max_n);
	while (A <= B) { //A>B�� max_n�� ����
		min_n = max_n;
		max_n *= 2;
		A = 10.0 * max_n * max_n;
		B = 300.0 * log2(max_n);
	}
	printf("�ʱ� min_n= %d, A:%.0f, B:%.0f, ", min_n, 10.0 * min_n * min_n, 300.0 * log2(min_n));
	printf("max_n= %d, A:%.0f, B:%.0f\n", max_n, 10.0 * max_n * max_n, 300.0 * log2(max_n));

	//n�� 8�� 16���̿��� ������ n�� ȿ�������� ���ϴ� �ڵ� �ۼ�(��ġ�ؼ� ����)
	while (max_n - min_n > 1) {
		n = (min_n + max_n) / 2;
		A = 10.0 * n * n;
		B = 300.0 * log2(n);
		if (A > B)
			max_n = n;
		else
			min_n = n;
	}
	printf("���� n= %d, A:%.0f, B:%.0f\n", max_n, 10.0 * max_n * max_n, 300.0 * log2(max_n));

}
int main(void)
{
	//3��
	prob_3();

	//6��
	prob_6();

	//13��
	prob_13();

    return 0;
}
/*���� ���
012, 013, 014, 023, 024, 034, 123, 124, 134, 234,
min:0, max:9
�ʱ� min_n= 8, A:640, B:900, max_n= 16, A:2560, B:1200
���� n= 10, A:1000, B:997
����Ϸ��� �ƹ� Ű�� �����ʽÿ� . . .
*/
/*�������


*/
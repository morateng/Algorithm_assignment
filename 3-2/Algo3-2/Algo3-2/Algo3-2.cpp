
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SIZE 10

//prob_14
void order(int i, int j, int P[][SIZE]) {
	int k;
	if (i == j)
		printf("A%d ", i);
	else {
		//아래를 완성하시오

	}
}
int minmult(int n, const int d[], int P[][SIZE]) {
	int i, j, k, diagonal;
	int min, min_k, tmp;
	int** M = new int* [n + 1];
	for (i = 1; i <= n; i++) {
		M[i] = new int[n + 1];
		M[i][i] = 0;
	}
	for (diagonal = 1; diagonal <= n - 1; diagonal++)
		for (i = 1; i <= n - diagonal; i++) {
			j = i + diagonal;
			min = INT_MAX;
			//아래를 완성하시오



			M[i][j] = min;
			P[i][j] = min_k;
		}
	return M[1][n];
}

void prob_14() {
	int n = 5;
	int d[] = { 10, 4, 5, 20, 2, 50 };
	int P[SIZE][SIZE];
	int result;

	result = minmult(n, d, P);
	printf("minulut:%d\n", result);
	printf("optimal oder:");
	order(1, n, P);
	printf("\n");
}

//prob_24
void obst(int p[], int q[], int cost[][SIZE], int root[][SIZE], int weight[][SIZE], int n) {
	int i, j, k, m, min, minpos;

	for (i = 0; i < n; i++) {
		weight[i][i] = q[i];
		root[i][i] = 0;
		cost[i][i] = 0;
		cost[i][i + 1] = weight[i][i + 1] = q[i] + q[i + 1] + p[i + 1];
		root[i][i + 1] = i + 1;
		printf("T%d%d: weight=%d, cost=%d, root=%d\n", i, i, weight[i][i], cost[i][i], root[i][i]);
		printf("T%d%d: weight=%d, cost=%d, root=%d\n", i, i + 1, weight[i][i + 1], cost[i][i + 1], root[i][i + 1]);
	}
	weight[n][n] = q[n];
	root[n][n] = 0;
	cost[n][n] = 0;
	printf("T%d%d: weight=%d, cost=%d, root=%d\n", n, n, weight[n][n], cost[n][n], root[n][n]);

	for (m = 2; m <= n; m++) {
		for (i = 0; i <= n - m; i++) {
			j = i + m;
			weight[i][j] = weight[i][j - 1] + p[j] + q[j];
			min = INT_MAX;
			minpos = 0;
			//아래를 완성하시오.




			cost[i][j] = weight[i][j] + min;
			root[i][j] = minpos;
			printf("T%d%d: weight=%d, cost=%d, root=%d\n", i, j, weight[i][j], cost[i][j], root[i][j]);
		}
	}
}

void order_obst(int i, int j, int root[][SIZE], const char* keys[]) {
	if (i < j) {
		printf("T%d%d: root:%d, key:%s, left subtree:T%d,%d, right subtree:T%d,%d",
			i, j, root[i][j], keys[root[i][j]], i, root[i][j] - 1, root[i][j], j);
		printf("\n");
		//아래를 완성하시오.


	}
}
void prob_24() {
	const char* keys[7] = { "", "CASE", "ELSE", "END", "IF", "OF", "THEN" };
	int p[] = { 0, 5, 15, 5, 35, 5, 35 };
	int q[] = { 0, 0, 0, 0, 0, 0, 0 };
	int n = 6;

	int cost[SIZE][SIZE], root[SIZE][SIZE], weight[SIZE][SIZE];

	obst(p, q, cost, root, weight, n);
	printf("cost=%d\n", cost[0][n]);

	order_obst(0, n, root, keys);
}

int main() {
	printf("\n문제 14.\n");
	prob_14();

	printf("\n문제 24.\n");
	prob_24();

	return 0;
}
/*실제출력


*/
/*예시 출력
문제 14.
minulut:1320
optimal oder:((A1 (A2 (A3 A4 )))A5 )

문제 24.
T00: weight=0, cost=0, root=0
T01: weight=5, cost=5, root=1
T11: weight=0, cost=0, root=0
T12: weight=15, cost=15, root=2
T22: weight=0, cost=0, root=0
T23: weight=5, cost=5, root=3
T33: weight=0, cost=0, root=0
T34: weight=35, cost=35, root=4
T44: weight=0, cost=0, root=0
T45: weight=5, cost=5, root=5
T55: weight=0, cost=0, root=0
T56: weight=35, cost=35, root=6
T66: weight=0, cost=0, root=0
T02: weight=20, cost=25, root=2
T13: weight=20, cost=25, root=2
T24: weight=40, cost=45, root=4
T35: weight=40, cost=45, root=4
T46: weight=40, cost=45, root=6
T03: weight=25, cost=35, root=2
T14: weight=55, cost=80, root=4
T25: weight=45, cost=55, root=4
T36: weight=75, cost=120, root=4
T04: weight=60, cost=95, root=4
T15: weight=60, cost=90, root=4
T26: weight=80, cost=130, root=4
T05: weight=65, cost=105, root=4
T16: weight=95, cost=165, root=4
T06: weight=100, cost=180, root=4
cost=180
T06: root:4, key:IF, left subtree:T0,3, right subtree:T4,6
T03: root:2, key:ELSE, left subtree:T0,1, right subtree:T2,3
T01: root:1, key:CASE, left subtree:T0,0, right subtree:T1,1
T23: root:3, key:END, left subtree:T2,2, right subtree:T3,3
T46: root:6, key:THEN, left subtree:T4,5, right subtree:T6,6
T45: root:5, key:OF, left subtree:T4,4, right subtree:T5,5
계속하려면 아무 키나 누르십시오 . . .
*/
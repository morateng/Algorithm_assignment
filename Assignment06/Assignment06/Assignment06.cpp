#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <functional>
using namespace std;

//베낭 문제
#define N_ITEMS 4 //아이템의 개수

struct node_knapsack {
	int level; //상태공간트리의 레벨, 고려하는 item을 나타냄
	int profit; //레벨(마디)에 오기까지 넣었던 아이템의 값어치의 합.
	int weight; //레벨(마디)에 오기까지 넣었던 아이템의 무게의 합.
	double bound; //레벨에서의 한계값
	bool include[N_ITEMS + 1];//아이템의 포함 여부
};

int maxprofit;//현재까지의 최대 이익
node_knapsack best_knapsack;//maxprofit를 만족하는 노드

int W = 16;//베낭의 허용가능 무게
int w[] = { 0, 2, 5, 10, 5 }; //각 아이템의 무게
int p[] = { 0, 40, 30, 50, 10 };//각 아이템의 값어치

struct cmp_knapsack { //우선순위큐에서 bound 값이 큰 순서대로 가져옴
	bool operator()(node_knapsack a, node_knapsack b) {//큰 값을 가진 노드순으로 삭제
		return a.bound < b.bound;
	}
};
std::priority_queue<node_knapsack, vector<node_knapsack>, cmp_knapsack> pq_knapsack;//시스템 제공 우선순위 큐

void bound_knapsack(node_knapsack* u) { //노드 u의 bound값을 계산
	int j;
	int totweight;

	if (u->weight > W) //노드 u의 무게가 베낭의 허용용량 초과
		u->bound = 0; //bound 값을 0으로 하여 유망성 없음 나타냄
	else { //bound 계산식을 적용함
		/*마디가 수준 i(u->level)에 있다고 하고, 수준 k에 있는 마디에서 총무게가 W를 넘는다고 하자. */
		u->bound = (double)u->profit;
		j = u->level + 1;
		totweight = u->weight;
		while (j <= N_ITEMS && totweight + w[j] <= W) {//아이템 j부터 차례대로 베낭에 넣음
			totweight += w[j];
			u->bound += p[j];
			j++;
		}
		//아래를 완성하시오. 
		//아이템 j 를 베낭에 추가시 허용량 초과로 잘라서 최대한 넣음(fractional knapsack)
		//따라서 j가 bound 계산식에서 k에 해당함
		if(j<=N_ITEMS){
			u->bound += (W - totweight) * (p[j] / w[j]);
		}
	}
}

void knapsack_bfs()
{
	node_knapsack u, v;

	v.level = 0; v.profit = 0; v.weight = 0; //초기화
	for (int i = 1; i <= N_ITEMS; i++)
		v.include[i] = false;
	bound_knapsack(&v);
	printf("삽입: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d\n",
		v.level, v.profit, v.weight, v.bound, maxprofit);
	pq_knapsack.push(v);

	while (!pq_knapsack.empty()) {
		v = pq_knapsack.top();
		pq_knapsack.pop();
		printf("삭제: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
			v.level, v.profit, v.weight, v.bound, maxprofit);
		if (v.bound > maxprofit)
			printf("-유망함\n");
		else
			printf("-유망하지 않음\n");
		if (v.bound > maxprofit) {
			for (int i = 1; i <= N_ITEMS; i++)
				u.include[i] = v.include[i];
			u.level = v.level + 1;

			u.weight = v.weight + w[u.level];
			u.profit = v.profit + p[u.level];
			u.include[u.level] = true; //아이템 베낭에 넣음
			bound_knapsack(&u);
			if (u.weight <= W && u.profit > maxprofit) {
				maxprofit = u.profit;
				best_knapsack = u;
			}
			if (u.bound > maxprofit)
				pq_knapsack.push(u);
			printf("\t왼쪽자식: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
				u.level, u.profit, u.weight, u.bound, maxprofit);
			if (u.bound > maxprofit)
				printf("-삽입\n");
			else
				printf("-버림\n");

			u.weight = v.weight;
			u.profit = v.profit;
			u.include[u.level] = false;//아이템 베낭에 넣지 않음
			bound_knapsack(&u);
			if (u.bound > maxprofit)
				pq_knapsack.push(u);
			printf("\t오른쪽자식: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
				u.level, u.profit, u.weight, u.bound, maxprofit);
			if (u.bound > maxprofit)
				printf("-삽입\n");
			else
				printf("-버림\n");
		}
	}
}

//외판원 문제

#define CITIES 5

struct node_tsp {//예:  u->path=[0,2,4], setA={1,3}, path_last=4, setA_last=1
	int path[CITIES];
	int path_last;//강의자료 bound 계산에서 k에 해당함. 현재까지 방문 도시에서 마지막 도시
	int setA[CITIES]; //아직까지 방문하지 못한 도시
	int setA_last;//setA(아직까지 방문하지 못한 도시)의 마지막 인덱스(크기-1)
	int distance; //현재까지 방문 도시들의 실제 거리
	int bound;//계산할 한계치
};
struct cmp_tsp {
	bool operator()(node_tsp a, node_tsp b) {//작은 값을 가진 노드순으로 삭제
		return a.bound > b.bound;
	}
};
std::priority_queue<node_tsp, vector<node_tsp>, cmp_tsp> pq_tsp;

node_tsp best_tsp;

int mat[CITIES][CITIES] =
{ {0,14,4,10,20},
 {14,0,7,8,7},
 {4,5,0,7,16},
 {11,7,9,0,2},
 {18,7,17,4,0} };

void last_path(node_tsp* u) {//방문하지 않은 1개 도시만 남음, u->path=[0,2,1,4], setA={3}, u->setA_last= 0
	int new_id = u->setA[u->setA_last]; //3
	u->distance += mat[u->path[u->path_last]][new_id];//4->3의 거리
	u->distance += mat[new_id][0];//3->0의 거리
	u->path_last++;
	u->path[u->path_last] = new_id;//u->path=[0,2,1,4,3]
	u->setA_last--;//setA={}
	u->bound = u->distance;
}

int bound_tsp(node_tsp u) {
	int i, j, min;
	int bound = u.distance;//[1,…,k] 경로 상의 총거리, k는  path_last에 해당
	int last_id = u.path[u.path_last]; //현재까지 방문 도시에서 마지막 도시

	//vk(last_id)에서 A(setA)에 속한 정점으로 가는 이음선의 길이들 중에서 최소치
	min = mat[last_id][u.setA[0]];
	for (i = 1; i <= u.setA_last; i++) {
		//아래를 완성하시오.
		if (min > mat[last_id][u.setA[i]]){
			min = mat[last_id][u.setA[i]];
		}

	}
	bound += min;

	//setA의 i에서 setA와 1의 합에서 i를 뺀 집합에 속한 정점으로 가는 이음선의 길이들 중에서 최소치
	for (i = 0; i <= u.setA_last; i++) {
		min = mat[u.setA[i]][0];
		for (j = 0; j <= u.setA_last; j++) {
			if (i != j && min > mat[u.setA[i]][u.setA[j]])
				min = mat[u.setA[i]][u.setA[j]];
		}
		bound += min;
	}

	return bound;
}
void print_tsp(node_tsp* u) {
	printf("distance=%d, bound=%d, [", u->distance, u->bound);
	for (int i = 0; i <= u->path_last; i++)
		printf("%d ", u->path[i]);
	printf("]\n");
}


int main()
{
	//베낭 문제 최고우선 검색
	printf("베낭 문제 최고우선 검색\n");
	printf("아이템 수 n: %d, 베낭 용량 W: %d\n", N_ITEMS, W);
	printf("i\tp\tw\tp/w\n");
	for (int i = 1; i <= N_ITEMS; i++) {
		printf("%d\t%d\t%d\t%d\n", i, p[i], w[i], p[i] / w[i]);
	}
	printf("\n");

	maxprofit = 0;
	knapsack_bfs();

	printf("\n");
	printf("최대이익:%d, 베낭무게:%d\n", best_knapsack.profit, best_knapsack.weight);
	printf("베낭에 포함되는 아이템: ");
	for (int i = 1; i <= N_ITEMS; i++) {
		if (best_knapsack.include[i])
			printf("아이템 %d, ", i);
	}
	printf("\n");


	//외판원 문제 최고우선 검색

	printf("\n외판원 문제 최고우선 검색\n");
	node_tsp u, v;

	best_tsp.distance = INT_MAX;
	u.path[0] = 0;
	u.path_last = 0;
	for (int i = 0; i < CITIES - 1; i++)
		u.setA[i] = i + 1;
	u.setA_last = CITIES - 2;
	u.distance = 0;
	u.bound = bound_tsp(u);

	printf("삽입:");
	print_tsp(&u);

	pq_tsp.push(u);

	while (!pq_tsp.empty()) {
		v = pq_tsp.top();
		pq_tsp.pop();
		printf("\t삭제:");
		print_tsp(&v);
		if (v.bound < best_tsp.distance) {//유망함
			u = v;
			for (int i = 0; i <= v.setA_last; i++) {
				u = v;
				u.path[++u.path_last] = u.setA[i];
				u.setA[i] = u.setA[u.setA_last--];
				u.distance += mat[u.path[u.path_last - 1]][u.path[u.path_last]];
				if (u.setA_last == 0) {//방문하지 않은 1개 도시만 남음
					last_path(&u);
					if (best_tsp.distance > u.distance)
						best_tsp = u;
				}
				else
					u.bound = bound_tsp(u);

				printf("삽입:");
				print_tsp(&u);
				if (u.bound < best_tsp.distance)
					pq_tsp.push(u);
			}
		}
	}
	printf("best tsp: ");
	print_tsp(&best_tsp);
	return 0;
}

/*실제출력
부분집합의 합 문제는 단위무게당 가치가 모두 같은 아이템을 사용하기 때문에
아이템의 무게가 작은 것부터 비내림차순으로 정렬하여 단순히 아이템의 넣고 안넣고에 따른 총 무게로 유망여부를 판단하였다.
배낭채우기문제는 아이템을 단위무게당 가치가 큰 순서부터 내림차순으로 정렬해 한계치 값으로 이용한다. 이를 이용하여 유망여부를 판단하여 상태공간트리를 생성한다.


베낭 문제 최고우선 검색
아이템 수 n: 4, 베낭 용량 W: 16
i       p       w       p/w
1       40      2       20
2       30      5       6
3       50      10      5
4       10      5       2

삽입: level=0, profit=0, weight=0, bound=115, maxprofit=0
삭제: level=0, profit=0, weight=0, bound=115, maxprofit=0-유망함
		왼쪽자식: level=1, profit=40, weight=2, bound=115, maxprofit=40-삽입
		오른쪽자식: level=1, profit=0, weight=0, bound=82, maxprofit=40-삽입
삭제: level=1, profit=40, weight=2, bound=115, maxprofit=40-유망함
		왼쪽자식: level=2, profit=70, weight=7, bound=115, maxprofit=70-삽입
		오른쪽자식: level=2, profit=40, weight=2, bound=98, maxprofit=70-삽입
삭제: level=2, profit=70, weight=7, bound=115, maxprofit=70-유망함
		왼쪽자식: level=3, profit=120, weight=17, bound=0, maxprofit=70-버림
		오른쪽자식: level=3, profit=70, weight=7, bound=80, maxprofit=70-삽입
삭제: level=2, profit=40, weight=2, bound=98, maxprofit=70-유망함
		왼쪽자식: level=3, profit=90, weight=12, bound=98, maxprofit=90-삽입
		오른쪽자식: level=3, profit=40, weight=2, bound=50, maxprofit=90-버림
삭제: level=3, profit=90, weight=12, bound=98, maxprofit=90-유망함
		왼쪽자식: level=4, profit=100, weight=17, bound=0, maxprofit=90-버림
		오른쪽자식: level=4, profit=90, weight=12, bound=90, maxprofit=90-버림
삭제: level=1, profit=0, weight=0, bound=82, maxprofit=90-유망하지 않음
삭제: level=3, profit=70, weight=7, bound=80, maxprofit=90-유망하지 않음

최대이익:90, 베낭무게:12
베낭에 포함되는 아이템: 아이템 1, 아이템 3,

외판원 문제 최고우선 검색
삽입:distance=0, bound=21, [0 ]
		삭제:distance=0, bound=21, [0 ]
삽입:distance=14, bound=31, [0 1 ]
삽입:distance=4, bound=22, [0 2 ]
삽입:distance=10, bound=30, [0 3 ]
삽입:distance=20, bound=42, [0 4 ]
		삭제:distance=4, bound=22, [0 2 ]
삽입:distance=9, bound=22, [0 2 1 ]
삽입:distance=20, bound=39, [0 2 4 ]
삽입:distance=11, bound=27, [0 2 3 ]
		삭제:distance=9, bound=22, [0 2 1 ]
삽입:distance=37, bound=37, [0 2 1 3 4 ]
삽입:distance=31, bound=31, [0 2 1 4 3 ]
		삭제:distance=11, bound=27, [0 2 3 ]
삽입:distance=43, bound=43, [0 2 3 1 4 ]
삽입:distance=34, bound=34, [0 2 3 4 1 ]
		삭제:distance=10, bound=30, [0 3 ]
삽입:distance=17, bound=45, [0 3 1 ]
삽입:distance=19, bound=38, [0 3 2 ]
삽입:distance=12, bound=30, [0 3 4 ]
		삭제:distance=12, bound=30, [0 3 4 ]
삽입:distance=30, bound=30, [0 3 4 1 2 ]
삽입:distance=48, bound=48, [0 3 4 2 1 ]
		삭제:distance=14, bound=31, [0 1 ]
		삭제:distance=20, bound=39, [0 2 4 ]
		삭제:distance=20, bound=42, [0 4 ]
best tsp: distance=30, bound=30, [0 3 4 1 2 ]

*/
/*예시 출력
베낭 문제 최고우선 검색
아이템 수 n: 4, 베낭 용량 W: 16
i       p       w       p/w
1       40      2       20
2       30      5       6
3       50      10      5
4       10      5       2

삽입: level=0, profit=0, weight=0, bound=115, maxprofit=0
삭제: level=0, profit=0, weight=0, bound=115, maxprofit=0-유망함
		왼쪽자식: level=1, profit=40, weight=2, bound=115, maxprofit=40-삽입
		오른쪽자식: level=1, profit=0, weight=0, bound=82, maxprofit=40-삽입
삭제: level=1, profit=40, weight=2, bound=115, maxprofit=40-유망함
		왼쪽자식: level=2, profit=70, weight=7, bound=115, maxprofit=70-삽입
		오른쪽자식: level=2, profit=40, weight=2, bound=98, maxprofit=70-삽입
삭제: level=2, profit=70, weight=7, bound=115, maxprofit=70-유망함
		왼쪽자식: level=3, profit=120, weight=17, bound=0, maxprofit=70-버림
		오른쪽자식: level=3, profit=70, weight=7, bound=80, maxprofit=70-삽입
삭제: level=2, profit=40, weight=2, bound=98, maxprofit=70-유망함
		왼쪽자식: level=3, profit=90, weight=12, bound=98, maxprofit=90-삽입
		오른쪽자식: level=3, profit=40, weight=2, bound=50, maxprofit=90-버림
삭제: level=3, profit=90, weight=12, bound=98, maxprofit=90-유망함
		왼쪽자식: level=4, profit=100, weight=17, bound=0, maxprofit=90-버림
		오른쪽자식: level=4, profit=90, weight=12, bound=90, maxprofit=90-버림
삭제: level=1, profit=0, weight=0, bound=82, maxprofit=90-유망하지 않음
삭제: level=3, profit=70, weight=7, bound=80, maxprofit=90-유망하지 않음

최대이익:90, 베낭무게:12
베낭에 포함되는 아이템: 아이템 1, 아이템 3,

외판원 문제 최고우선 검색
삽입:distance=0, bound=21, [0 ]
		삭제:distance=0, bound=21, [0 ]
삽입:distance=14, bound=31, [0 1 ]
삽입:distance=4, bound=22, [0 2 ]
삽입:distance=10, bound=30, [0 3 ]
삽입:distance=20, bound=42, [0 4 ]
		삭제:distance=4, bound=22, [0 2 ]
삽입:distance=9, bound=22, [0 2 1 ]
삽입:distance=20, bound=39, [0 2 4 ]
삽입:distance=11, bound=27, [0 2 3 ]
		삭제:distance=9, bound=22, [0 2 1 ]
삽입:distance=37, bound=37, [0 2 1 3 4 ]
삽입:distance=31, bound=31, [0 2 1 4 3 ]
		삭제:distance=11, bound=27, [0 2 3 ]
삽입:distance=43, bound=43, [0 2 3 1 4 ]
삽입:distance=34, bound=34, [0 2 3 4 1 ]
		삭제:distance=10, bound=30, [0 3 ]
삽입:distance=17, bound=45, [0 3 1 ]
삽입:distance=19, bound=38, [0 3 2 ]
삽입:distance=12, bound=30, [0 3 4 ]
		삭제:distance=12, bound=30, [0 3 4 ]
삽입:distance=30, bound=30, [0 3 4 1 2 ]
삽입:distance=48, bound=48, [0 3 4 2 1 ]
		삭제:distance=14, bound=31, [0 1 ]
		삭제:distance=20, bound=39, [0 2 4 ]
		삭제:distance=20, bound=42, [0 4 ]
best tsp: distance=30, bound=30, [0 3 4 1 2 ]
계속하려면 아무 키나 누르십시오 . . .
*/
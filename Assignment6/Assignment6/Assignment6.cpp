#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <functional>
using namespace std;

//���� ����
#define N_ITEMS 4 //�������� ����

struct node_knapsack {
	int level; //���°���Ʈ���� ����, ����ϴ� item�� ��Ÿ��
	int profit; //����(����)�� ������� �־��� �������� ����ġ�� ��.
	int weight; //����(����)�� ������� �־��� �������� ������ ��.
	double bound; //���������� �Ѱ谪
	bool include[N_ITEMS + 1];//�������� ���� ����
};

int maxprofit;//��������� �ִ� ����
node_knapsack best_knapsack;//maxprofit�� �����ϴ� ���

int W = 16;//������ ��밡�� ����
int w[] = { 0, 2, 5, 10, 5 }; //�� �������� ����
int p[] = { 0, 40, 30, 50, 10 };//�� �������� ����ġ

struct cmp_knapsack { //�켱����ť���� bound ���� ū ������� ������
	bool operator()(node_knapsack a, node_knapsack b) {//ū ���� ���� �������� ����
		return a.bound < b.bound;
	}
};
std::priority_queue<node_knapsack, vector<node_knapsack>, cmp_knapsack> pq_knapsack;//�ý��� ���� �켱���� ť

void bound_knapsack(node_knapsack* u) { //��� u�� bound���� ���
	int j;
	int totweight;

	if (u->weight > W) //��� u�� ���԰� ������ ���뷮 �ʰ�
		u->bound = 0; //bound ���� 0���� �Ͽ� ������ ���� ��Ÿ��
	else { //bound ������ ������
		/*���� ���� i(u->level)�� �ִٰ� �ϰ�, ���� k�� �ִ� ���𿡼� �ѹ��԰� W�� �Ѵ´ٰ� ����. */
		u->bound = (double)u->profit;
		j = u->level + 1;
		totweight = u->weight;
		while (j <= N_ITEMS && totweight + w[j] <= W) {//������ j���� ���ʴ�� ������ ����
			totweight += w[j];
			u->bound += p[j];
			j++;
		}
		//�Ʒ��� �ϼ��Ͻÿ�. 
		//������ j �� ������ �߰��� ��뷮 �ʰ��� �߶� �ִ��� ����(fractional knapsack)
		//���� j�� bound ���Ŀ��� k�� �ش���


	}
}

void knapsack_bfs()
{
	node_knapsack u, v;

	v.level = 0; v.profit = 0; v.weight = 0; //�ʱ�ȭ
	for (int i = 1; i <= N_ITEMS; i++)
		v.include[i] = false;
	bound_knapsack(&v);
	printf("����: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d\n",
		v.level, v.profit, v.weight, v.bound, maxprofit);
	pq_knapsack.push(v);

	while (!pq_knapsack.empty()) {
		v = pq_knapsack.top();
		pq_knapsack.pop();
		printf("����: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
			v.level, v.profit, v.weight, v.bound, maxprofit);
		if (v.bound > maxprofit)
			printf("-������\n");
		else
			printf("-�������� ����\n");
		if (v.bound > maxprofit) {
			for (int i = 1; i <= N_ITEMS; i++)
				u.include[i] = v.include[i];
			u.level = v.level + 1;

			u.weight = v.weight + w[u.level];
			u.profit = v.profit + p[u.level];
			u.include[u.level] = true; //������ ������ ����
			bound_knapsack(&u);
			if (u.weight <= W && u.profit > maxprofit) {
				maxprofit = u.profit;
				best_knapsack = u;
			}
			if (u.bound > maxprofit)
				pq_knapsack.push(u);
			printf("\t�����ڽ�: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
				u.level, u.profit, u.weight, u.bound, maxprofit);
			if (u.bound > maxprofit)
				printf("-����\n");
			else
				printf("-����\n");

			u.weight = v.weight;
			u.profit = v.profit;
			u.include[u.level] = false;//������ ������ ���� ����
			bound_knapsack(&u);
			if (u.bound > maxprofit)
				pq_knapsack.push(u);
			printf("\t�������ڽ�: level=%d, profit=%d, weight=%d, bound=%g, maxprofit=%d",
				u.level, u.profit, u.weight, u.bound, maxprofit);
			if (u.bound > maxprofit)
				printf("-����\n");
			else
				printf("-����\n");
		}
	}
}

//���ǿ� ����

#define CITIES 5

struct node_tsp {//��:  u->path=[0,2,4], setA={1,3}, path_last=4, setA_last=1
	int path[CITIES];
	int path_last;//�����ڷ� bound ��꿡�� k�� �ش���. ������� �湮 ���ÿ��� ������ ����
	int setA[CITIES]; //�������� �湮���� ���� ����
	int setA_last;//setA(�������� �湮���� ���� ����)�� ������ �ε���(ũ��-1)
	int distance; //������� �湮 ���õ��� ���� �Ÿ�
	int bound;//����� �Ѱ�ġ
};
struct cmp_tsp {
	bool operator()(node_tsp a, node_tsp b) {//���� ���� ���� �������� ����
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

void last_path(node_tsp* u) {//�湮���� ���� 1�� ���ø� ����, u->path=[0,2,1,4], setA={3}, u->setA_last= 0
	int new_id = u->setA[u->setA_last]; //3
	u->distance += mat[u->path[u->path_last]][new_id];//4->3�� �Ÿ�
	u->distance += mat[new_id][0];//3->0�� �Ÿ�
	u->path_last++;
	u->path[u->path_last] = new_id;//u->path=[0,2,1,4,3]
	u->setA_last--;//setA={}
	u->bound = u->distance;
}

int bound_tsp(node_tsp u) {
	int i, j, min;
	int bound = u.distance;//[1,��,k] ��� ���� �ѰŸ�, k��  path_last�� �ش�
	int last_id = u.path[u.path_last]; //������� �湮 ���ÿ��� ������ ����

	//vk(last_id)���� A(setA)�� ���� �������� ���� �������� ���̵� �߿��� �ּ�ġ
	min = mat[last_id][u.setA[0]];
	for (i = 1; i <= u.setA_last; i++) {
		//�Ʒ��� �ϼ��Ͻÿ�.


	}
	bound += min;

	//setA�� i���� setA�� 1�� �տ��� i�� �� ���տ� ���� �������� ���� �������� ���̵� �߿��� �ּ�ġ
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
	//���� ���� �ְ�켱 �˻�
	printf("���� ���� �ְ�켱 �˻�\n");
	printf("������ �� n: %d, ���� �뷮 W: %d\n", N_ITEMS, W);
	printf("i\tp\tw\tp/w\n");
	for (int i = 1; i <= N_ITEMS; i++) {
		printf("%d\t%d\t%d\t%d\n", i, p[i], w[i], p[i] / w[i]);
	}
	printf("\n");

	maxprofit = 0;
	knapsack_bfs();

	printf("\n");
	printf("�ִ�����:%d, ��������:%d\n", best_knapsack.profit, best_knapsack.weight);
	printf("������ ���ԵǴ� ������: ");
	for (int i = 1; i <= N_ITEMS; i++) {
		if (best_knapsack.include[i])
			printf("������ %d, ", i);
	}
	printf("\n");


	//���ǿ� ���� �ְ�켱 �˻�

	printf("\n���ǿ� ���� �ְ�켱 �˻�\n");
	node_tsp u, v;

	best_tsp.distance = INT_MAX;
	u.path[0] = 0;
	u.path_last = 0;
	for (int i = 0; i < CITIES - 1; i++)
		u.setA[i] = i + 1;
	u.setA_last = CITIES - 2;
	u.distance = 0;
	u.bound = bound_tsp(u);

	printf("����:");
	print_tsp(&u);

	pq_tsp.push(u);

	while (!pq_tsp.empty()) {
		v = pq_tsp.top();
		pq_tsp.pop();
		printf("\t����:");
		print_tsp(&v);
		if (v.bound < best_tsp.distance) {//������
			u = v;
			for (int i = 0; i <= v.setA_last; i++) {
				u = v;
				u.path[++u.path_last] = u.setA[i];
				u.setA[i] = u.setA[u.setA_last--];
				u.distance += mat[u.path[u.path_last - 1]][u.path[u.path_last]];
				if (u.setA_last == 0) {//�湮���� ���� 1�� ���ø� ����
					last_path(&u);
					if (best_tsp.distance > u.distance)
						best_tsp = u;
				}
				else
					u.bound = bound_tsp(u);

				printf("����:");
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

/*�������


*/
/*���� ���
���� ���� �ְ�켱 �˻�
������ �� n: 4, ���� �뷮 W: 16
i       p       w       p/w
1       40      2       20
2       30      5       6
3       50      10      5
4       10      5       2

����: level=0, profit=0, weight=0, bound=115, maxprofit=0
����: level=0, profit=0, weight=0, bound=115, maxprofit=0-������
		�����ڽ�: level=1, profit=40, weight=2, bound=115, maxprofit=40-����
		�������ڽ�: level=1, profit=0, weight=0, bound=82, maxprofit=40-����
����: level=1, profit=40, weight=2, bound=115, maxprofit=40-������
		�����ڽ�: level=2, profit=70, weight=7, bound=115, maxprofit=70-����
		�������ڽ�: level=2, profit=40, weight=2, bound=98, maxprofit=70-����
����: level=2, profit=70, weight=7, bound=115, maxprofit=70-������
		�����ڽ�: level=3, profit=120, weight=17, bound=0, maxprofit=70-����
		�������ڽ�: level=3, profit=70, weight=7, bound=80, maxprofit=70-����
����: level=2, profit=40, weight=2, bound=98, maxprofit=70-������
		�����ڽ�: level=3, profit=90, weight=12, bound=98, maxprofit=90-����
		�������ڽ�: level=3, profit=40, weight=2, bound=50, maxprofit=90-����
����: level=3, profit=90, weight=12, bound=98, maxprofit=90-������
		�����ڽ�: level=4, profit=100, weight=17, bound=0, maxprofit=90-����
		�������ڽ�: level=4, profit=90, weight=12, bound=90, maxprofit=90-����
����: level=1, profit=0, weight=0, bound=82, maxprofit=90-�������� ����
����: level=3, profit=70, weight=7, bound=80, maxprofit=90-�������� ����

�ִ�����:90, ��������:12
������ ���ԵǴ� ������: ������ 1, ������ 3,

���ǿ� ���� �ְ�켱 �˻�
����:distance=0, bound=21, [0 ]
		����:distance=0, bound=21, [0 ]
����:distance=14, bound=31, [0 1 ]
����:distance=4, bound=22, [0 2 ]
����:distance=10, bound=30, [0 3 ]
����:distance=20, bound=42, [0 4 ]
		����:distance=4, bound=22, [0 2 ]
����:distance=9, bound=22, [0 2 1 ]
����:distance=20, bound=39, [0 2 4 ]
����:distance=11, bound=27, [0 2 3 ]
		����:distance=9, bound=22, [0 2 1 ]
����:distance=37, bound=37, [0 2 1 3 4 ]
����:distance=31, bound=31, [0 2 1 4 3 ]
		����:distance=11, bound=27, [0 2 3 ]
����:distance=43, bound=43, [0 2 3 1 4 ]
����:distance=34, bound=34, [0 2 3 4 1 ]
		����:distance=10, bound=30, [0 3 ]
����:distance=17, bound=45, [0 3 1 ]
����:distance=19, bound=38, [0 3 2 ]
����:distance=12, bound=30, [0 3 4 ]
		����:distance=12, bound=30, [0 3 4 ]
����:distance=30, bound=30, [0 3 4 1 2 ]
����:distance=48, bound=48, [0 3 4 2 1 ]
		����:distance=14, bound=31, [0 1 ]
		����:distance=20, bound=39, [0 2 4 ]
		����:distance=20, bound=42, [0 4 ]
best tsp: distance=30, bound=30, [0 3 4 1 2 ]
����Ϸ��� �ƹ� Ű�� �����ʽÿ� . . .
*/
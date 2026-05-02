#include <iostream>
#include<vector>
using namespace std;

struct Player {//record와 id를 기록할 player구조 생성
	int id;
	int record;
};

//player객체를 넣는 벡터 v1,빈공간의 위치 인덱스,옮겨야할 노드 K,전체 노드 개수 N를 인자로 받는다
void fixheap(vector<Player>& v1, int vacant, Player K, int N) {//fixheap 구현(construct heap용)
	if (vacant * 2 > N) {    //vacant가 리프인 경우,N이 전체 노드의 개수로 구현
		v1[vacant] = K;
	}
	else {
		int smallerChild;//최소 힙이므로
		if (2 * vacant == N) {
			// 왼쪽 자식 하나뿐인 경우
			smallerChild = 2 * vacant;
		}
		else {
			// 자식이 둘 다 있는 경우 (2 * vacant < N)
			if (v1[2 * vacant].record <= v1[2 * vacant + 1].record)
				smallerChild = 2 * vacant;
			else
				smallerChild = 2 * vacant + 1;
		}
		//smallerChild와 옮겨야할 노드 K값을 비교
		if (v1[smallerChild].record > K.record) {//K가 더 작다면 vacant에 K대입
			v1[vacant] = K;
		}
		else {//그렇지 않다면 smallerChild를 vacant위치로 바꿔주고
			v1[vacant] = v1[smallerChild];
			fixheap(v1, smallerChild, K, N);//vacant의 위치를 smallerChild로 바꿔 재귀호출
		}


	}
}


//v1벡터 만들고, 루트를 표현하기위한 index,전체 노드개수 N
void constructHeap(vector<Player>&v1,int root,int N) {
	if (root*2+1 <=N) {//자식이 있는지 확인 있을때만 실행
		constructHeap(v1, root * 2,N);
		constructHeap(v1, root * 2 + 1,N);
		Player K = v1[root];//바꿔야되는 위치의 루트를 K로 해서 fixheap에 넣어줌
		fixheap(v1, root, K, N);
	}
}

void fastheapSort(vector<Player>& v1,int N) {
	for (int i = N;i >= 1;i--) {
		Player curMin = v1.erase(1);
	}
}

void promte() {//vacant를 이동

}

void fixHeapFast(vector<Player>& v1,int vacant,Player K,int h,int N) {//heapsort용 fixheapfast
	if (h <= 1) {
		
	}
	else {
		int hStop = h / 2;
		int vacStop=promote(v1,hStop,vacant,h)
	}
}



int main() {
	int T{ 0 };
	vector <Player> v1;//Player힙 배열을 만들 vector구성
	v1.push_back({-1,-1});//vector의 인덱스 0에 더미 값 삽입
	cin >> T;

	for (int i = 0;i < T;i++) {//T번 반복
		int N = 0;
		cin >> N;
		for(int j=0;j<N;j++){//힙에 내용물 삽입
			int pid, pr;
			cin >> pid >> pr;
			Player player1(pid, pr);
			v1.push_back(player1);
		}
		constructHeap(v1, 1, N);//힙 구조 생성
		for (int k = 1;i < N + 1;k++) {//minheap을 이용하여 오름차순 정렬
			fastheapSort(v1, N);

		}
		
		v1.clear();//한번의 반복이 끝난 후 벡터 초기화
	}
}

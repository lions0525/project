#include <iostream>
#include<vector>
using namespace std;
long long TotalTime = 0;

struct Player {//record와 id를 기록할 player구조 생성
	int id;
	int record;
	Player(int i, int r) {
		id = i;
		record = r;
	}
};
//함수 선언 부분
void fixHeapFast(vector<Player>& v1, int vacant, Player K, int h, int N, bool isSort);

//fixheapfast에서 h가 1이하일때를 위한 함수
//player객체를 넣는 벡터 v1,빈공간의 위치 인덱스,옮겨야할 노드 K,N은 접근할 수 있는 유효한 마지막 인덱스를 의미
void fixheap(vector<Player>& v1, int vacant, Player K, int N, bool isSort) {
	if (vacant * 2 > N) {    //vacant가 리프인 경우,N이 마지막 유효한 인덱스이므로 자식이 N보다 크면 리프다
		v1[vacant] = K; //fixheap에서 vacant가 리프이므로 현재 자리에 k삽입
	}
	else {
		int smallerChild;//최소 힙이므로
		if (2 * vacant == N) {
			// 왼쪽 자식 하나뿐인 경우
			smallerChild = 2 * vacant;
		}
		else {
			// 자식이 둘 다 있는 경우 (2 * vacant+1 <= N)
			if (v1[2 * vacant].record <= v1[2 * vacant + 1].record)//오른쪽 자식이 더 크면 왼쪽 자식을 smaller에
				smallerChild = 2 * vacant;
			else
				smallerChild = 2 * vacant + 1;
		}

		//smallerChild와 옮겨야할 노드 K값을 비교
		if (v1[smallerChild].record > K.record) {//K가 더 작다면 vacant에 K대입
			v1[vacant] = K;
		}

		else {//그렇지 않다면 smallerChild를 vacant위치로 바꿔주고,smaller child인덱스를 vacant로 설정
			v1[vacant] = v1[smallerChild];
			if (isSort) {
				TotalTime += 1;//vacant가 아래로 이동
			}
			fixheap(v1, smallerChild, K, N, isSort);//vacant의 위치를 smallerChild로 바꿔 재귀호출
		}
	}
}
int getHeight(int root_idx, int N) {//힙의 높이 구하는 함수,루트의 인덱스와 전체 유효 인덱스를 통해 구함
	int h = 0;
	int sum = root_idx;
	while (sum * 2 <= N) {//인덱스*2가 유효인덱스보다 작은 경우 자식이 있고 높이 1을 더 해줄수 있다
		sum *= 2;
		h++;
	}
	return h;
}
//v1벡터 만들고, 루트를 표현하기위한 index,유효인덱스를 나타내는 N
void constructHeap(vector<Player>& v1, int root, int N) {
	if (root * 2 <= N) {//자식이 있는지 확인,있을때만 실행
		constructHeap(v1, root * 2, N);//왼쪽 자식을 루트로 하는 서브트리에서 실행
		constructHeap(v1, root * 2 + 1, N);//오른쪽 자식을 루트로 하는 서브트리에서 실행
		Player K = v1[root];//바꿔야되는 위치의 루트를 K로 해서 fixheap에 넣어줌
		int h = getHeight(root, N);
		fixHeapFast(v1, root, K, h, N, false);//호출 당시 높이를 넣어줘야한다,constructheap에서는 항상 false
	}
}
void bubbleUpHeap(vector<Player>& v1, int vacant, Player K, bool isSort) {//처음 이동한 vacant에서의 값을 비교
	if (vacant == 1) {//vacant가 루트에 위치하는 경우, vacant인덱스에 K를 넣어주고 끝
		v1[vacant] = K;
	}
	else {
		int vacparent = vacant / 2;
		if (K.record >= v1[vacparent].record) {//vacparent가 더 작은 경우, 현재 위치에 K대입
			v1[vacant] = K;
		}
		else {//vacant가 더 작은 경우 vacparent를 vacant로 내려주고 재귀실행
			v1[vacant] = v1[vacparent];
			if (isSort) {//vacant의 깊이가 1칸 위로 이동
				TotalTime += 1;
			}
			bubbleUpHeap(v1, vacparent, K, isSort);
		}
	}
}
int promote(vector<Player>& v1, int hStop, int vacant, int h, int N, bool isSort) {//vacant의 위치를 한번에 낮춰주는 함수
	int vacStop;
	if (h <= hStop || vacant * 2 > N) {//목표 높이에 도달했거나 자식이 없으면 정지
		vacStop = vacant;
	}
	else if (vacant * 2 == N) {//왼쪽 자식만 있는경우
		v1[vacant] = v1[vacant * 2];
		if (isSort) {//vacant가 아래로 이동
			TotalTime += 1;
		}
		vacStop = 2 * vacant;//힙상 왼쪽 자식만 있는 경우엔 더 내려갈 곳이 없으므로,vacStop을 바꿔주고 끝
	}
	else {//자식이 둘일 경우
		if (v1[2 * vacant].record <= v1[2 * vacant + 1].record) {//왼쪽이 더 작을 때
			v1[vacant] = v1[2 * vacant];
			if (isSort) {//vacant가 아래로 이동
				TotalTime += 1;
			}
			vacStop = promote(v1, hStop, 2 * vacant, h - 1, N, isSort);
		}
		else {//오른쪽이 더 작을 때
			v1[vacant] = v1[2 * vacant + 1];
			if (isSort) {
				TotalTime += 1;//vacant가 아래로 이동
			};
			vacStop = promote(v1, hStop, 2 * vacant + 1, h - 1, N, isSort);
		}
	}
	return vacStop;
}
void fixHeapFast(vector<Player>& v1, int vacant, Player K, int h, int N, bool isSort) {//heapsort용 fixheapfast
	if (h <= 1) {//높이가 1 이하인 경우,promote를 사용하더라도 어차피 한층의 두번의 비교가 필요,따라서 직접 비교로 불필요한 계산을 줄인다
		fixheap(v1, vacant, K, N, isSort);
	}
	else {
		int hStop = h / 2;//한번에 내릴 목표 설정
		int vacStop = promote(v1, hStop, vacant, h, N, isSort);//promote함수를 통해 vacStop인덱스를 구함
		int vacParent = vacStop / 2;
		if (v1[vacParent].record > K.record) {//부모가 K보다 큰경우
			v1[vacStop] = v1[vacParent];
			if (isSort) {
				TotalTime += 1;//vacant의 깊이가 위로 이동
			}
			bubbleUpHeap(v1, vacParent, K, isSort);
		}
		else {
			int newh = getHeight(vacStop, N);
			fixHeapFast(v1, vacStop, K, hStop, N, isSort);
		}
	}
}
vector<int> fastheapSort(vector<Player>& v1, int N) {//heap에서 정렬을 시키는 함수
	vector<int> rankTable;//리턴 해줄 등수표
	rankTable.push_back({ -1 });//등수표 인덱스 0에 더미값 삽입
	for (int i = N;i > 1;i--) {//노드의 개수만큼 계산해줘야한다,이때 노드의 개수가 1개인 경우 바로 넣으므로 2이상일때만
		Player curMin = v1[1];//루트 값(최솟값) 저장
		v1[1] = v1[i];//맨 마지막 노드를 루트로 이동
		rankTable.push_back(curMin.id);//등수표의 최솟값의 id삽입
		int h = getHeight(1, i - 1); //남은 노드의 높이 계산
		TotalTime += 1;//vacant의 깊이가 0으로 이동
		fixHeapFast(v1, 1, v1[1], h, i - 1, true);
	}

	rankTable.push_back(v1[1].id);
	return rankTable;
}


int main() {
	ios_base::sync_with_stdio(false);//타임 리미트를 해결하기 위해서 속도 증가
	cin.tie(NULL);
	int T{ 0 };
	cin >> T;
	for (int i = 0;i < T;i++) {//T번 반복
		vector <Player> v1;//Player힙 배열을 만들 vector구성
		v1.push_back({ -1,-1 });//vector의 인덱스 0에 더미 값 삽입,초기화를 해주므로 계속 넣어줘야함
		TotalTime = 0;//수행시간 초기화
		int N;
		cin >> N;
		for (int j = 0;j < N;j++) {//힙에 내용물 삽입,내용물 개수 설정
			int pid, pr;
			cin >> pid >> pr;
			Player player1(pid, pr);
			v1.push_back(player1);
		}
		constructHeap(v1, 1, N);//힙 구조 생성
		for (int k = 1;k < N + 1;k++) {
			cout << v1[k].id << " ";
		}
		cout << "\n";
		vector<int> rankTable = fastheapSort(v1, N);//힙 정렬
		for (int k = 1;k < N + 1;k++) {//힙 정렬후 출력
			cout << rankTable[k] << " ";
		}
		cout << "\n";
		cout << TotalTime << "\n";
		v1.clear();//한번의 반복이 끝난 후 벡터 초기화
	}
}

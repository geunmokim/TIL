#include "framework.h"
#include "PathFind.h"
#include "TileMgr.h" //is_wall을 활용하기 위해
#include <queue>

CPathFind::CPathFind()
{

}
CPathFind::~CPathFind()
{

}

GRIDPOS CPathFind::Find_NextStep(const GRIDPOS& _start,
	const GRIDPOS& _target)          //_start -> 몬스터의 시작점 _target -> 플레이어의 위치
{

	if (_start == _target)   // 이미 같은 칸이면, 더 갈 곳도 없으니 그대로 반환
		return _start;

	bool visited[GRID_ROW_MAX][GRID_COL_MAX] = {}; //방문 기록 //bfs의 탐색 도중의 중복확인 방지 
	GRIDPOS parent[GRID_ROW_MAX][GRID_COL_MAX]; //경로 역추적용 기록 //이칸이 어디서 왔는지 ->bfs는 도착결과만 알려주고 과정은 안알려주는데 몬스터가 어느 방향으로 가야할지 필요하기때문에

	queue<GRIDPOS> q; //탐색 좌표 저장용  //큐를 사용하는 이유는 fifo로 먼저 탐색한곳을 방문한다는 특성 때문

	visited[_start.row][_start.col] = true; //입력 받은 시작 좌표를 방문
	q.push(_start); //몬스터 위치에서 출발

	int dRow[] = { -1,1,0,0, -1,-1,1,1 }; //상하좌우 대각선 세로 방향
	int dCol[] = { 0,0,-1,1, -1,1,-1,1 }; //상하좌우 가로 방향

	// 저장된 모든 좌표를 탐색할때까지 반복
	while (!q.empty())
	{
		//que의 front 좌표를, 현재 좌표로 지정
		GRIDPOS current = q.front();

		//front 좌표를 위에서 현재 좌표로 저장했으므로 front의 좌표는 삭제
		q.pop();

		if (current == _target) //플레이어의 위치에 도달했는가? //사실상 네비게이션으로 목적지만 찍은셈 실제위치는 아직 안변함
		{
			GRIDPOS step = _target;

			while (!(parent[step.row][step.col] == _start)) //출발지점이 될때까지 거슬러 올라간다 //거슬러 올라가는 이유는 목적지를 알았으니 출발지점에서 어느칸으로 움직일지 정하려고
			{
				step = parent[step.row][step.col];  //경로 역추적용 기록 parent로 한칸씩 거슬러 올라감 -> 시작지점에 도달할때까지
				
			}
			return step;
		}

		//현재 좌표와 상하좌우로 인접한 좌표 확인
		for (int i = 0; i < 8; ++i)
		{
			int nextRow = current.row + dRow[i];
			int nextCol = current.col + dCol[i];

			//인접한 좌표가 범위 안인지 //GRID_ROW_MAX, GRID_COL_MAX가 던전의 세로 가로 크기
			if (nextRow < 0 || nextRow >= GRID_ROW_MAX)  //범위 밖이라면
				continue;    //방문하지 않는다
			if (nextCol < 0 || nextCol >= GRID_COL_MAX)
				continue;    //방문하지 안흔ㄴ다
			if (CTileMgr::Get_Instance()->Is_Wall(nextRow, nextCol))//벽이라면
				continue;   //방문하지 않는다
			if (visited[nextRow][nextCol])  //방문기록 visited로 방문한적이 있다면(true라면)
				continue;   //방문하지 않는다 

			//위의 조건들에 false라면
			q.push(GRIDPOS(nextRow, nextCol)); //다음에 방문할 지점이 nextRow,nextCol라고 push
			visited[nextRow][nextCol] = true; //다음에 방문할 시 true로 continue 되게
			parent[nextRow][nextCol] = current; //추적용 기록에 현재의 좌표를 넣어둠 (parent는 이전 좌표를 저장하는곳이니까)





		}

	}
	return _start; //길을 찾지 못했다면 시작지점에 머무르게
}
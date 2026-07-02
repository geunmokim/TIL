#pragma once

class CPathFind 
{
private:
	CPathFind();  //객체 생성이 필요없기에 생성자,소멸자 모두 private 처리
	~CPathFind(); //굳이 안써도 자동 호출되지만 코드 스타일 통일을 위해 작성

public:
	//플레이어의 위치를 받아 몬스터가 어디로 가야할지
	static GRIDPOS Find_NextStep(const GRIDPOS& _start, const GRIDPOS& _target);


	

//private:
//
//	//방문 기록 //bfs의 탐색 도중의 중복확인 방지 
//	 bool visited[GRID_ROW_MAX][GRID_COL_MAX];    ->Find_NextStep이 객체생성할 필요가 없게끔 static으로 전환하면서 멤버 변수들을 전부 제거하고 Find_NextStep의 지역변수로 들어가게 함
//
//	//경로 역추적용 기록 //이칸이 어디서 왔는지 ->bfs는 도착결과만 알려주고 과정은 안알려주는데 몬스터가 어느 방향으로 가야할지 필요하기때문에
//	 GRIDPOS parent[GRID_ROW_MAX][GRID_COL_MAX]; ->위의 visited와 마찬가지의 이유로 주석처리



};











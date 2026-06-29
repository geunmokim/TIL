#pragma once
#define WINCX 800 //창의 가로 크기(x축)
#define WINCY 600 //창의 세로 크기(y축)



#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}  //function.h에서 template을 활용한 Safe_delete매크로를 만들기 떄문에 사실 이건 없어도 되는 코드다

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1      

#define PI 3.14159f  //라디안과 각도 사용을 위한 pi 정의



#define TILECX 64 //타일 하나의 가로 크기
#define TILECY 64 //타일 하나의 세로 크기
#define TILEX 30 //타일의 가로개수로 쓰일 것
#define TILEY 20 //타일의 세로개수로 쓰일 것


#define TILE_SRC_SIZE 26 //새로

// 타일 에디터에서 이 번호 이상의 DrawID를 가진 타일은 "벽(이동 불가)"으로 취급한다.
// 예: 0~9번 DrawID는 바닥/장식용, 10번부터는 벽 그림으로 배치한다는 규칙.
#define WALL_DRAWID 9

//기존 플레이어의 헤더에 있던걸 pathfind에서도 사용하기 위해 define.h로 이동
#define GRID_ROW_MAX TILEY
#define GRID_COL_MAX TILEX

// 몬스터가 플레이어를 발견(인지)할 수 있는 거리(체비셰프 거리 기준).
// 임시값. 나중에 시야(VisionMgr) 시스템과 연동하면 더 정교해질 수 있다.
#define MONSTER_SIGHT_RANGE 5

//Sight에서 사용할 플레이어의 시야 거리
#define PLAYER_SIGHT_RANGE 6
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


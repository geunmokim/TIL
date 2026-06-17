#pragma once
#define WINCX 800 //창의 가로 크기(x축)
#define WINCY 600 //창의 세로 크기(y축)



#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}  //function.h에서 template을 활용한 Safe_delete매크로를 만들기 떄문에 사실 이건 없어도 되는 코드다

#define OBJ_NOEVENT 0
#define OBJ_DEAD 1      

#define PI 3.14159f


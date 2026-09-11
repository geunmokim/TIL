#pragma once
namespace Engine
{
	typedef struct tagVertexCubeTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vTexUV;
	}VTXCUBETEX;

	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORTEX;

	//vertex(정점)구조체 3종 -> gpu에 넘길 정점 뎅이터 형식
	//dx11에서 3D 물체를 그리려면, 각 정점(꼭짓점)이 어떤 정보를 담고 있는지 미리 구조체로 정의해서
	//GPU(버텍스 셰이더)에 넘겨줘야 한다. 3종인 이유는 용도 별로 필요한 정보가 달라서 3개를 사용
	//VTXCUBETEX(위치 + UV): 큐브(정육면체) 텍스처 매핑용. 조명 계산이 필요없는 단순한 물체에 사용
	//VTXTEX(위치 + UV): 가장 기본적인, 텍스처만 입히는 정점(조명/ 노말 없음)
	//VTXNORTEX(위치 + 노말 + UV): 조명 계산이 필요한 물체용. 노말(법선 벡터)이 있어야 빛이 표면에 어떻게 반사되는지 계산 가능

	typedef struct tagFaceIndices16
	{
		unsigned short		_1, _2, _3;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_1, _2, _3;
	}FACEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short		_1, _2;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_1, _2;
	}LINEINDICES32;

	//인덱스 구조체 4종 - > 정점을 어떻게 연결해서 도형을 만들지
	//3d 모델은 정점 목록만으로는 어떤 정점끼리 연결해서 면을 만들지 알 수 없다
	//-> 인덱스 버퍼로 몇번쨰 정점과 몇번쨰 정점을 이어라 를 따로 지정해준다
	//FACEINDICES16 /FACEINDICES32(_1,_2,_3 -> 3개):삼각형(면)하나를 구성 하는 정ㅈ엄 3개의 인덱스. 
	//3D모델은 결국 삼각형들의 집합응로 그려지기 떄문에 필요
	//삼각형들로 그리는 이유는 삼각형은 항상 평면ㅇ ㅣ 보장되고 어떤 다각형이든 결국 삼각형들로 쪼갤 수 있기 때문
	//LINEINDICES16/ LINEINDICES32(_1,_2 -> 2개):선(와이어프레임, 디버그용 라인 등)하나를 구성하는 정점 2개의 인덱스
	//16과 32로 나뉜 이유는 정점 개수에 따라 메모리를 아끼기 위해서
	//->unsigned short(16비트) 용과 unsigned long(32비트) 용


	typedef struct tagInputDesc
	{
		HINSTANCE			hInst;
		HWND				hWnd;
	} INPUTDESC;
	//입력 시스템 초기화용ㅇ

	typedef struct tagGraphicDesc
	{
		HWND				hWnd;
		bool				isWindowed;
		unsigned int		iWinCX, iWinCY;
	} GRAPHICDESC;
}

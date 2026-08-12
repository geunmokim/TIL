#pragma once
#include <d3d11.h>

//day7 이미지 파일을 읽어서 DX11이 그릴 수 있는 텍스처로 만들어주는 담당
class CTexture
{
	public:
		//성공하면 SRV(그리기용 뷰)를 리턴, 실패하면 nullptr
		//pOutWidth/pOutHeight에 원본 이미지의 가로/ 세로 픅셀 크기를 담앗허 돌려줌
		static ID3D11ShaderResourceView* Load_Texture(const char* pFilePath, int* pOutWidth, int* pOutHeight);

};

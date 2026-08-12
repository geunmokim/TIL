#define STB_IMAGE_IMPLEMENTATION	// stb_image.h를 "구현체 포함"으로 한 번만 include
#include "stb_image.h"

#include "Device.h"
#include "Texture.h"

ID3D11ShaderResourceView* CTexture::Load_Texture(const char* pFilePath, int* pOutWidth, int* pOutHeight)
{
	int iWidth = 0, iHeight = 0, iChannels = 0;

	// [Day7] 파일에서 픽셀 데이터를 읽어옴. 4는 "무조건 RGBA 4채널로 통일해서 달라"는 뜻
	unsigned char* pPixels = stbi_load(pFilePath, &iWidth, &iHeight, &iChannels, 4);
	if (nullptr == pPixels)
		return nullptr;

	// D3D11 텍스처 만들 준비 (Device.cpp에서 하던 것과 비슷한 패턴)
	D3D11_TEXTURE2D_DESC TexDesc = {};
	TexDesc.Width = iWidth;
	TexDesc.Height = iHeight;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA SubData = {};
	SubData.pSysMem = pPixels;
	SubData.SysMemPitch = iWidth * 4;	// 한 줄(row)이 몇 바이트인지 (가로픽셀 * 4채널)

	ID3D11Texture2D* pTexture = nullptr;
	CDevice::Get_Instance()->Get_Device()->CreateTexture2D(&TexDesc, &SubData, &pTexture);

	stbi_image_free(pPixels);	// 픽셀 원본 데이터는 텍스처로 복사됐으니 이제 필요 없음, 메모리 해제

	if (nullptr == pTexture)
		return nullptr;

	// ImGui::Image에 넘길 SRV(Shader Resource View) 생성
	ID3D11ShaderResourceView* pSRV = nullptr;
	CDevice::Get_Instance()->Get_Device()->CreateShaderResourceView(pTexture, nullptr, &pSRV);
	pTexture->Release();	// SRV가 텍스처를 붙잡고 있으니, 원본 텍스처 포인터는 여기서 놔줘도 됨

	if (pOutWidth)	*pOutWidth = iWidth;
	if (pOutHeight)	*pOutHeight = iHeight;

	return pSRV;
}

/*
* 다 알 필요는없고 대충 흐름만 파악하기
1. stbi_load(...)              → 파일을 읽어서 "픽셀 값들이 쭉 나열된 배열"로 만듦 (CPU 메모리에 있음)
2. D3D11_TEXTURE2D_DESC 채우기  → "이런 크기, 이런 형식의 텍스처를 만들어줘" 설정값 뭉치 (Device.cpp의 SwapChainDesc랑 똑같은 패턴)
3. CreateTexture2D(...)        → 그 픽셀 데이터를 실제 GPU 메모리로 복사해서 텍스처로 만듦
4. stbi_image_free(...)        → CPU에 있던 원본 픽셀 배열은 이제 필요 없으니 메모리 해제
5. CreateShaderResourceView(...) → ImGui::Image가 쓸 수 있는 형태(SRV)로 한 번 더 감싸기

개념적으로는 기억해두면 좋은 것: "CPU에 있는 이미지 파일 → GPU가 그릴 수 있는 텍스처"로 가는 과정에서 항상 '복사'가 일어난다는 것

*/
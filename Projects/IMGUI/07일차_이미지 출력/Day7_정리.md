# Day7 정리: 이미지 출력

## 1. 새로 추가된 것

| 항목 | 역할 |
|---|---|
| `stb_image.h` | PNG/JPG 파일을 픽셀 데이터로 읽어주는 외부 라이브러리 (ImGui엔 이미지 로딩 기능이 없음) |
| `Texture.h/cpp` (새 파일) | 픽셀 데이터를 DX11 텍스처로 만들고, `ImGui::Image`가 쓸 수 있는 SRV 형태로 변환 |
| `ImGui::ImageButton` | 텍스트 대신 이미지를 그리는 클릭 가능한 버튼 |

## 2. 텍스처 로딩 흐름 (Device.cpp처럼 "이런 게 있다" 정도로 알고 넘어가는 파트)

```
stbi_load(...)                  → 파일 읽어서 픽셀 배열로 (CPU 메모리)
D3D11_TEXTURE2D_DESC 채우기      → "이런 크기/형식 텍스처 만들어줘" 설정값 뭉치
CreateTexture2D(...)            → 픽셀 데이터를 GPU 메모리로 복사해서 텍스처 생성
stbi_image_free(...)            → CPU 쪽 원본 픽셀 배열은 이제 필요없으니 해제
CreateShaderResourceView(...)   → ImGui::Image가 쓸 수 있는 SRV로 한 번 더 감싸기
```
핵심 개념 하나만: **"CPU에 있는 이미지 파일 → GPU가 그릴 수 있는 텍스처"로 가려면 항상 복사가 일어난다.** 이건 나중에 다른 이미지 로딩 라이브러리를 써도 똑같이 적용되는 개념.

## 3. include 관련 재확인 사항

`Texture.cpp`에서 `CDevice::Get_Instance()`를 쓰려면 `#include "Device.h"`가 필요함 — 컴파일러가 `CDevice`라는 클래스와 그 함수들의 존재를 알아야 하기 때문. `<d3d11.h>` 빠뜨렸을 때(Day2)랑 같은 원리: **"내가 쓰는 건 내가 직접 include"**.

```cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Device.h"      // CDevice 쓰려면 필요
#include "Texture.h"
```

## 4. 실행 파일 기준 경로 개념

`stbi_load("test.png", ...)`처럼 상대 경로로 파일을 찾을 때, 그 기준은 "exe가 있는 폴더"가 아니라 **VS가 F5로 디버그 실행할 때의 작업 폴더**(보통 `.vcxproj`가 있는 프로젝트 폴더)임. `x64\Debug\exe`를 직접 더블클릭해서 실행할 땐 기준이 달라지므로, 그 경우엔 그 폴더에도 이미지 파일이 따로 있어야 함.

## 5. `ImGui::Image` vs `ImGui::ImageButton`
- `Image` — 그냥 보여주기만, 클릭 안 됨
- `ImageButton` — 이미지 자체가 버튼, 클릭하면 true 리턴 (이번에 쓴 것)

## 확인한 것
그리드의 칸1이 텍스트 대신 실제 PNG 이미지로 나오고, 클릭하면 기존 텍스트 버튼들(칸2~6)이랑 똑같이 `m_iSelectedSlot`에 저장되어 "선택: 칸1"로 반영되는 것까지 확인.

## Day8 예고
지금 `m_Monsters` 배열에 있는 데이터를 **실제 파일로 저장하고, 다시 불러오는 것**을 배울 예정. 지금까지는 프로그램을 끄면 수정한 값이 다 날아갔는데, Day8부터는 그게 유지됨.

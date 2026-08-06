#include <windows.h>
#include <d3d11.h>
#include "MainGame.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CMainGame)

CMainGame::CMainGame()
	:m_bCheck(false)
	, m_fValue(0.0f)
{

}

CMainGame::~CMainGame()
{

}

HRESULT CMainGame::Init_ImGui(HWND hWnd)
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	// 기본 폰트엔 한글 글리프가 없어서 전부 ?로 나옴 -> 윈도우 기본 맑은고딕을 한글 범위로 로드
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, nullptr,
		io.Fonts->GetGlyphRangesKorean());

	ImGui::StyleColorsDark();

	// Win32(입력 처리) + DX11(렌더링), 두 백엔드를 각각 초기화
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::Get_Instance()->Get_Device(),
		CDevice::Get_Instance()->Get_Context());

	return S_OK;
}

void CMainGame::Update(void)
{
	// 매 프레임 제일 먼저 호출
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ---- 여기부터 실제로 "그리는" 코드 ----
	ImGui::Begin(u8"Day3 테스트 창");

	ImGui::Text(u8"기본 위젯 3개 테스트");

	if (ImGui::Button(u8"버튼"))
		m_fValue = 0.0f;			// 버튼 누르면 슬라이더 값 초기화

	ImGui::Checkbox(u8"체크박스", &m_bCheck);

	ImGui::SliderFloat(u8"슬라이더", &m_fValue, 0.0f, 100.0f);	//라벨(창에 뜨는 이름)  연결할 변수 주소   최소값   최대값

	if (m_bCheck)
		ImGui::Text(u8"체크됨! 값: %.1f", m_fValue);

	ImGui::End();
}

void CMainGame::Render(void)
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CMainGame::Release(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//update()안의 위젯 함수들은 계속 내용을 바꿔가며 사용하기 떄문에 알아두고
//나머지것들은 이런것이 있다 정도로만 알기

/*
아래는 외부에서 다운 받아온 imgui의 파일들 
imgui.h / imgui.cpp — 핵심 본체. ImGui::Button(), ImGui::Begin() 같은 함수들이 실제로 정의된 곳.
imgui_widgets.cpp — 버튼, 슬라이더, 체크박스, 콤보박스 같은 위젯 하나하나의 그리기/입력처리 코드.
imgui_draw.cpp — 위젯들을 실제 화면에 그릴 삼각형/정점 데이터로 변환하는 저수준 그리기 로직.
imgui_tables.cpp — 표(테이블), 여러 칸으로 나누는 레이아웃(Columns 포함) 관련 코드.
imgui_demo.cpp — "Dear ImGui Demo" 창의 소스. 위젯 백과사전, 없어도 프로그램은 돌아가지만 참고용으로 꼭 넣는 걸 추천.
imgui_internal.h — 위 cpp들끼리 내부적으로 공유하는 비공개 함수/구조체 모음. 우리가 직접 쓸 일은 거의 없고, 그냥 컴파일에 필요해서 넣는 것.
imconfig.h — ImGui 동작을 커스터마이징할 수 있는 설정 파일 (지금은 기본값 그대로 씀).
imstb_rectpack.h, imstb_textedit.h, imstb_truetype.h — 폰트 렌더링/텍스트 입력 처리에 쓰는 외부 유틸리티 라이브러리 3개.
*/
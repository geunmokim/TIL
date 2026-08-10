#include <windows.h>
#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MainGame.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CMainGame)

CMainGame::CMainGame()
	:m_iSelectedSlot(-1)
	,m_iSelectedMonster(0)
	,m_iMonsterType(0)
{
	//day6 배열 초기값 3마리 다르게 채워둠(구분되게)
	m_Monsters[0] = { "Ghoul",   80,  12.5f, false, { 0.6f, 0.2f, 0.2f } };
	m_Monsters[1] = { "Skeleton",60,  8.0f,  false, { 0.7f, 0.7f, 0.7f } };
	m_Monsters[2] = { "Ogre",    200, 25.0f, true,  { 0.3f, 0.5f, 0.2f } };

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

	//day4 창 하나 안 몰아넣고, 역할별로 나눠서 각각 호출
	Draw_ToolbarWindow();
	Draw_MonsterListWindow(); //day3에 있던 update()내용들은 이곳에 옮겨놓음
	Draw_GridWindow();

}

void CMainGame::Draw_ToolbarWindow(void)
{
	//day4 창이 처음 생성될 때(저장된 위치 없을 때)만 이 좌표에서 시작
	//->나중에 사용자가 드래그해서 옮기면, 그 다음부턴 이 코드가 무시되고, imgui.ini에 저장된 위치를 사용함
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);


	ImGui::Begin(u8"툴바");
	ImGui::Text(u8"버튼 3개를 가로로 나열 (SameLine)");

	if (ImGui::Button(u8"버튼A")) {}
	ImGui::SameLine();		// 다음 위젯을 줄바꿈 없이 바로 옆에 이어붙임
	if (ImGui::Button(u8"버튼B")) {}
	ImGui::SameLine();
	if (ImGui::Button(u8"버튼C")) {}

	ImGui::End();
}

void CMainGame::Draw_MonsterListWindow(void)
{
	// Day3에서 만들었던 위젯들을 그대로 여기로 옮김
	ImGui::SetNextWindowPos(ImVec2(20, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 420), ImGuiCond_FirstUseEver);

	ImGui::Begin(u8"몬스터 목록");

	//day6 ListBox: 몬스터 이름들을 목록으로 보여주고, 클릭한 인덱스를 리턴
	//세 번쨰 인자에 넘기는 배열은 const char* 배열이라, 매 프레임 이름을 다시 뽑아서 채워줘야 함
	const char* NameList[MONSTER_COUNT];
	for (int i = 0; i < MONSTER_COUNT; ++i)
		NameList[i] = m_Monsters[i].Name;

	ImGui::ListBox(u8"목록", &m_iSelectedMonster, NameList, MONSTER_COUNT, 4);
		//                    선택 인덱스(주소), 문자열배열, 배열개수    ,화면에 몇줄 보일지

	ImGui::Separator();

	//day6 Combo: ListBox랑 비슷한데, 평소엔 접혀있다가 클릭하면 펼쳐지는 드롭다운
	static const char* TypeNames[] = { u8"언데드", u8"야수",u8"거인" };
	ImGui::Combo(u8"종족", &m_iMonsterType, TypeNames, IM_ARRAYSIZE(TypeNames));

	ImGui::Separator();

	//리스트에서 선택된 몬스터를 편집(day5에서 배운 위젯들 재사용)
	MonsterData& Selected = m_Monsters[m_iSelectedMonster]; //참조로 받아서 바로 수정되게끔

	ImGui::Text(u8"선택된 몬스터 편집");
	ImGui::InputText(u8"이름", Selected.Name, sizeof(Selected.Name));
	ImGui::InputInt(u8"체력(HP)", &Selected.Hp);
	ImGui::SliderFloat(u8"공격력(ATK)", &Selected.Atk, 0.0f, 100.0f);
	ImGui::Checkbox(u8"대형(Large)", &Selected.bLarge);

	//day6 TreeNode:클릭하면 접혔다 펼쳐졌다 하는 그룹 영역
    //자주 사용하지 않는 세부 설정을 평소에 접어두고 싶을 떄 사용
	if (ImGui::TreeNode(u8"색상 상세 설정"))
	{
		ImGui::ColorEdit3(u8"색상", Selected.Color);
		ImGui::TreePop();	// TreeNode를 열었으면(true) 반드시 짝으로 TreePop() 호출
	}

	ImGui::End();
}

void CMainGame::Draw_GridWindow(void)
{
	// [Day4] 창 크기까지 같이 지정. 위치만 주면 내용물 크기에 맞춰 너무 좁게 잡힐 수 있음.
	ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 220), ImGuiCond_FirstUseEver);

	ImGui::Begin(u8"그리드");

	// BeginChild: 창 안에 스크롤 가능한 작은 영역을 하나 더 만듦
	ImGui::BeginChild(u8"슬롯영역", ImVec2(0, 140), true);

	static const char* SlotNames[] = { u8"칸1", u8"칸2", u8"칸3", u8"칸4", u8"칸5", u8"칸6" };

	// Columns: 가로로 N칸 나눔
	ImGui::Columns(3, u8"슬롯칼럼", false);
	for (int i = 0; i < IM_ARRAYSIZE(SlotNames); ++i)
	{
		bool bSelected = (m_iSelectedSlot == i);
		if (bSelected)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 0.3f, 1.0f));

		if (ImGui::Button(SlotNames[i], ImVec2(-FLT_MIN, 40)))
			m_iSelectedSlot = i;

		if (bSelected)
			ImGui::PopStyleColor();

		ImGui::NextColumn();
	}
	ImGui::Columns(1);	// 컬럼 레이아웃 종료 (안 닫으면 이후 위젯도 계속 3칸으로 나뉨)

	ImGui::EndChild();

	ImGui::Separator();
	if (m_iSelectedSlot >= 0)
		ImGui::Text(u8"선택: %s", SlotNames[m_iSelectedSlot]);
	else
		ImGui::Text(u8"선택 없음");

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
()안에 u8을 붙이는 이유 -> /utf-8이 맞다는 걸 인식시켜주기 위해서 
utf-8 -> 한글 텍스트를 사용하기때문에 그걸 인식 시키기 위해서 옵션에서 사용 
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
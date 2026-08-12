#include <windows.h>
#include <d3d11.h>

#include <commdlg.h>                 //GetSaveFileName, GetOpenFileName
#include <cstdio>                   //fopen, fwrite, fread
#pragma comment(lib, "comdlg32.lib")

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "MainGame.h"
#include "Device.h"
#include "Texture.h"


IMPLEMENT_SINGLETON(CMainGame)

//day9 팔레트/맵 공용 - 타일 종류별 이름과 색상
static const char* SlotNames[] = { u8"잔디", u8"돌", u8"물", u8"모래", u8"길", u8"벽" };
static const ImVec4 TileColors[] = {
		ImVec4(0.3f, 0.6f, 0.2f, 1.0f),	// 잔디
	ImVec4(0.5f, 0.5f, 0.5f, 1.0f),	// 돌
	ImVec4(0.2f, 0.4f, 0.8f, 1.0f),	// 물
	ImVec4(0.8f, 0.7f, 0.4f, 1.0f),	// 모래
	ImVec4(0.6f, 0.5f, 0.3f, 1.0f),	// 길
	ImVec4(0.3f, 0.3f, 0.3f, 1.0f),	// 벽
};

CMainGame::CMainGame()
	:m_iSelectedSlot(0)    //day9 -> -1(선택없음)대신 0으로: 맵 찍을 땐 항상 타일 하나가 선택된 상태여야 함
	,m_iSelectedMonster(0)
	,m_iMonsterType(0)
	,m_pIconTexture(nullptr)  
{
	//day6 배열 초기값 3마리 다르게 채워둠(구분되게)
	m_Monsters[0] = { "Ghoul",   80,  12.5f, false, { 0.6f, 0.2f, 0.2f } };
	m_Monsters[1] = { "Skeleton",60,  8.0f,  false, { 0.7f, 0.7f, 0.7f } };
	m_Monsters[2] = { "Ogre",    200, 25.0f, true,  { 0.3f, 0.5f, 0.2f } };

	//day9 추가 맵 전체를 0번 타일(잔디)로 초기화
	for (int y = 0; y < MAP_HEIGHT; ++y)
		for (int x = 0; x < MAP_WIDTH; ++x)
			m_MapData[y][x] = 0;

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

	// [Day7 추가] 이 시점엔 CDevice가 이미 초기화 끝난 상태라 텍스처 로드 가능
	m_pIconTexture = CTexture::Load_Texture("test.png", &m_iIconWidth, &m_iIconHeight);
	if (nullptr == m_pIconTexture)
		ERR_MSG(L"텍스처 로드 실패 - test.png가 실행 파일과 같은 폴더에 있는지 확인하세요");

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
	Draw_MapWindow();

}

void CMainGame::Draw_ToolbarWindow(void)
{
	//day4 창이 처음 생성될 때(저장된 위치 없을 때)만 이 좌표에서 시작
	//->나중에 사용자가 드래그해서 옮기면, 그 다음부턴 이 코드가 무시되고, imgui.ini에 저장된 위치를 사용함
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);


	ImGui::Begin(u8"툴바");
	//day8 버튼 A/B/C였던 자리에 실제 기능 연결
	if (ImGui::Button(u8"저장"))
		Save_Monsters();
	ImGui::SameLine();    	// ← 이 줄이 없으면 "불러오기" 버튼이 아래 줄로 감
	if (ImGui::Button(u8"불러오기"))
		Load_Monsters();
	ImGui::SameLine();
	if (ImGui::Button(u8"초기화"))
	{
		m_Monsters[0] = { "Ghoul",   80,  12.5f, false, { 0.6f, 0.2f, 0.2f } };
		m_Monsters[1] = { "Skeleton",60,  8.0f,  false, { 0.7f, 0.7f, 0.7f } };
		m_Monsters[2] = { "Ogre",    200, 25.0f, true,  { 0.3f, 0.5f, 0.2f } };
	}

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
	//Day9 그리드였던 이 창이 이제 타일 팔레트 역할을 함 
	ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(320, 220), ImGuiCond_FirstUseEver);

	ImGui::Begin(u8"그리드");
	ImGui::Text(u8"타일 팔레트 - 클릭해서 찍을 타일 선택");

	// BeginChild: 창 안에 스크롤 가능한 작은 영역을 하나 더 만듦
	ImGui::BeginChild(u8"슬롯영역", ImVec2(0, 140), true);

	/*static const char* SlotNames[] = { u8"칸1", u8"칸2", u8"칸3", u8"칸4", u8"칸5", u8"칸6" };*/

	// Columns: 가로로 N칸 나눔
	ImGui::Columns(3, u8"슬롯칼럼", false);
	for (int i = 0; i < IM_ARRAYSIZE(SlotNames); ++i)
	{
		bool bSelected = (m_iSelectedSlot == i);

		// [Day9] 텍스트/색상 버튼으로 타일 종류를 구분 (이미지 텍스처는 0번 칸에서만 데모)
		ImGui::PushStyleColor(ImGuiCol_Button, TileColors[i]);
		if (bSelected)
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		
		if (0 == i && m_pIconTexture)
		{
			if (ImGui::ImageButton(u8"칸1_이미지", (ImTextureID)m_pIconTexture, ImVec2(40, 40)))
				m_iSelectedSlot = i;
		}
		else
		{
			if (ImGui::Button(SlotNames[i], ImVec2(-FLT_MIN, 40)))
				m_iSelectedSlot = i;
		}

		if (bSelected)
			ImGui::PopStyleColor();

		ImGui::PopStyleColor();

		ImGui::NextColumn();
	}
	ImGui::Columns(1);	// 컬럼 레이아웃 종료 (안 닫으면 이후 위젯도 계속 3칸으로 나뉨)

	ImGui::EndChild();

	ImGui::Separator();

	ImGui::Text(u8"선택된 타일: %s", SlotNames[m_iSelectedSlot]);



	ImGui::End();
}
void CMainGame::Draw_MapWindow(void)
{
	// [Day9 추가] 팔레트에서 고른 타일을 실제로 찍는 맵 격자
	ImGui::SetNextWindowPos(ImVec2(360, 260), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(360, 320), ImGuiCond_FirstUseEver);

	ImGui::Begin(u8"맵");

	if (ImGui::Button(u8"맵 저장"))
		Save_Map();
	ImGui::SameLine();
	if (ImGui::Button(u8"맵 불러오기"))
		Load_Map();

	ImGui::Separator();

	ImGui::Columns(MAP_WIDTH, u8"맵칼럼", false);
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			ImGui::PushID(y * MAP_WIDTH + x);	// 버튼 라벨이 다 똑같아서(""), ID 안 겹치게 고유 ID 부여

			int iTile = m_MapData[y][x];
			ImGui::PushStyleColor(ImGuiCol_Button, TileColors[iTile]);

			// 클릭하면 현재 팔레트에서 고른 타일(m_iSelectedSlot)로 이 칸을 칠함
			if (ImGui::Button("", ImVec2(30, 30)))
				m_MapData[y][x] = m_iSelectedSlot;

			ImGui::PopStyleColor();
			ImGui::PopID();

			ImGui::NextColumn();
		}
	}
	ImGui::Columns(1);

	ImGui::End();
}

void CMainGame::Render(void)
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CMainGame::Release(void)
{

	if (m_pIconTexture)
	{
		m_pIconTexture->Release();
		m_pIconTexture = nullptr;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void CMainGame::Save_Monsters(void)
{
	//day8 win32 표준 "다른 이름으로 저장" 대화 상자 
	wchar_t FilePath[MAX_PATH] = L"monsters.dat";


	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Monster Data (*.dat)\0*.dat\0";
	ofn.lpstrFile = FilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = L"dat";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn))
		return;		// 사용자가 취소 누름

	// 몬스터 배열을 통째로 파일에 씀 (구조체 그대로 바이너리로)
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath, L"wb");
	if (nullptr == pFile)
	{
		ERR_MSG(L"파일 저장 실패");
		return;
	}

	fwrite(m_Monsters, sizeof(MonsterData), MONSTER_COUNT, pFile);
	fclose(pFile);
}

void CMainGame::Load_Monsters(void)
{
	wchar_t FilePath[MAX_PATH] = L"";

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Monster Data (*.dat)\0*.dat\0";
	ofn.lpstrFile = FilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath, L"rb");
	if (nullptr == pFile)
	{
		ERR_MSG(L"파일 열기 실패");
		return;
	}

	fread(m_Monsters, sizeof(MonsterData), MONSTER_COUNT, pFile);
	fclose(pFile);

	m_iSelectedMonster = 0;	// 선택 인덱스가 범위 밖 될 일 없게 안전하게 초기화
}

void CMainGame::Save_Map(void)
{
	// [Day9 추가] Save_Monsters랑 완전히 같은 패턴, 대상만 m_MapData로 바뀜
	wchar_t FilePath[MAX_PATH] = L"map.dat";

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Map Data (*.dat)\0*.dat\0";
	ofn.lpstrFile = FilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = L"dat";
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath, L"wb");
	if (nullptr == pFile)
	{
		ERR_MSG(L"맵 저장 실패");
		return;
	}

	// 2차원 배열이지만 메모리상 한 덩어리로 이어져 있어서 한 번에 통째로 쓸 수 있음
	fwrite(m_MapData, sizeof(int), MAP_WIDTH * MAP_HEIGHT, pFile);
	fclose(pFile);
}

void CMainGame::Load_Map(void)
{
	wchar_t FilePath[MAX_PATH] = L"";

	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = L"Map Data (*.dat)\0*.dat\0";
	ofn.lpstrFile = FilePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath, L"rb");
	if (nullptr == pFile)
	{
		ERR_MSG(L"맵 불러오기 실패");
		return;
	}

	fread(m_MapData, sizeof(int), MAP_WIDTH * MAP_HEIGHT, pFile);
	fclose(pFile);
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
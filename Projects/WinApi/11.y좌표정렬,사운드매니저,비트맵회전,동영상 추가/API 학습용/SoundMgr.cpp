#include "framework.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr()
	:m_pSystem(nullptr)
{
	ZeroMemory(m_pChannelArr, sizeof(m_pChannelArr));
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);

	//1.시스템 포인터, 2.사용할 가상채널 수, 초기화 방식)
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}
void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;                //Mymapir.first 즉, key값은 	map<TCHAR*, FMOD_SOUND*> m_mapSound;로 
		                                      //TCHAR* pSoundKey = new TCHAR[iLength];   // LoadSoundFile()에서 이렇게 만들어짐
		                                      //직접 만들었기때문에 delete로 직접 해제해줘야한다
		
		
		FMOD_Sound_Release(Mypair.second);   //반면 second 즉, value값은 fmod 라이브러리 내부에서 만들어준것이기 떄문에 FMOD가 만든 걸 FMOD 함수로 해제하게 한다

	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem); //레거시 코드를 가져왔지만 CLOSE가 먼저 그 후 RELEASE 하는게 맞다
}

void CSoundMgr::PlaySound(const TCHAR* pSound, CHANNELID eID)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	
	iter = find_if(
		m_mapSound.begin(),    // 인자 1: 시작 반복자
		m_mapSound.end(),      // 인자 2: 끝 반복자
		[&](auto& iter)        // 인자 3: 람다 (여기서부터 시작)  //캡쳐목록 []에 & -> 람다바깥의 변수(pSoundKey)를 가져와 쓰겠다는 뜻 
		{
			return !lstrcmp(pSound, iter.first); //pSoundKey는 바깥 변수라 여기선 못 씀(전역이 아니면) -> 바깥 변수를 함께 들고 다니는 즉석 함수가 필요
			                                       //그래서 [&]를 사용한것 //람다 바깥의 변수를 가져와 사용하겠다는 의미로 
		}
	);
	//map<TCHAR*, FMOD_SOUND*>를 map<wstring,FMOD_SOUND*>로 사용했다면 auto iter = m_mapSound.find(pSound); 이거 한줄로 끝났을 것 

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;                                //FMOD_BOOL은 int와 같다고 보면 된다 
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay)) //재생 중인지 여부를 판별
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);

	/*
	//아래 대로 하는게 원래 의도대로 호출 성공여부와 진짜 재생중인지를 판별할 수 있게된다
	FMOD_BOOL bPlay = FALSE; 
    FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);   // 호출만 하고

    if (!bPlay)   // 진짜 재생 중인지(bPlay)를 직접 검사
    {
    FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
    }
	*/


}
void CSoundMgr::PlayBGM(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}
void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = _findfirst("../Sound/*.*", &fd); //_findfirst는 실패시 -1을 반환한다 그래서 -1을 체크해야함

	if (handle == -1) //_findfirst는 실패시 -1을 반환하므로 -1로 체크
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;      

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength); // 만약 변환 후 글자 수가 byte 수보다 많아지는 인코딩이면 버퍼 오버런 위험이 있다

			/*
			MultiByteToWideChar(CP_ACP, 0, fd.name, -1, NULL, 0)으로 필요한 길이를 먼저 구한 다음 그 크기로 버퍼를 할당하는게 맞다
			*/

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

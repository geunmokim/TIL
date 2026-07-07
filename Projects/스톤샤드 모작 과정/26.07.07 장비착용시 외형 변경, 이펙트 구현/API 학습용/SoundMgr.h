#pragma once
class CSoundMgr
{
public:
	enum CHANNELID {BGM, PLAYER,MONSTER,EFFECT, UI, MAXCHANNEL};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;
		return m_pInstance;

	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void Initialize();
	void Release();
public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID);  //객체를 호출해서 PlaySound를 불러올때 매개변수가 문자열 리터럴이면 CONST를 안붙여줄시 에러생김
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;

	//사운드 리소스 정보를 갖는 객체
	map< TCHAR*, FMOD_SOUND*> m_mapSound;  //map<wstring, FMOD_SOUND*>를 사용하면 밑에 주석 달아놓은 부분들을 해결 가능하다 //wstring은 문자열끼리 비교하기 떄문에 find사용이 가능해져서
	/*
	현재 THCAR* 로 포인터를 사용해 문자열 비교가 아닌 문자열의 주소를 사용하는데 
	new로 할당받는 주소는 그날그날 메모리 상황에 따라 달라지므로
	같은 파일명이라도 실행할 때마다 트리 안에서 위치가 달라진다
	->map이 이름순으로 정렬해서 빨리 찾아주는 본래기능을 제대로 사용 못하고있는것
	문자열 주소를 비교하기 때문에 map::find를 사용못하고 find_if로 처음부터 끝까지 순회하며 찾음 -> wstring을 사용하면 해결
	*/
	//FMOD_CHANNEL:재생하고 있는 사운드를 관리할 객체
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	//사운드, 채널 객체 및 장치를 관리하는 객체
	FMOD_SYSTEM* m_pSystem;







};


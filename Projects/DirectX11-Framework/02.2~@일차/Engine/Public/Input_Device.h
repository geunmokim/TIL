#pragma once

//입력 장치(키보드/마우스)를 다루는 클래스 
//왜 필요한가?
//매 프레임마다 입력을 확인하는 것이 필요한데 이걸 여기저기서 각자 호출하게 두면 지저분해지고 일관성도 없게됨
//-> 그래서 입력을 담당하는 ㅊㅏㅇ구 하나를 만드는데 기능으로는
//1.매 프레임 키보드/마우스 상태를 한 번에 읽어와서
//2.내부 배열/변수에 저장해두고
//3.다른 코드는 이 클래스에 "지금 A키 눌렸어?"라고 물어보기만 하면 되는 구조 로 만든다



#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };
	enum MOUSEBUTTONSTATE { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte Get_DIKeyState(_ubyte eKeyID) {
		return m_byKeyState[eKeyID];
	}

	_long Get_DIMouseMoveState(MOUSEMOVESTATE eMouseMoveState) {
		return *((_long*)&m_MouseState + eMouseMoveState);
	}

	_byte Get_DIMouseButtonState(MOUSEBUTTONSTATE eMouseButtonState) {
		return m_MouseState.rgbButtons[eMouseButtonState];
	}


public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_InputDeviceState();

private:
	LPDIRECTINPUT8				m_pInput = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;
private:
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;

public:
	virtual void Free() override;
};

END


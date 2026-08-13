#pragma once

#include "Client_Defines.h"

//class CTest
//{
//public:
//	/* 생성자 */
//	/* 소멸자 = default */
//
//public: 
//	/* Getter */
//	/* Setter */
//
//public:
//	/* 함수 */
//protected:
//	/* 변수 */
//protected:
//	/* 함수 */
//private:
//	/* 변수 */
//private:
//	/* 함수 */
//
//public:
//	/* 생성관련된 함수. */
//	/* 삭제관련된 함수. */
//};




namespace Client
{
	class CMainApp
	{
	private:
		CMainApp();
		~CMainApp();
	public:
		static CMainApp* Create();

	public:
		virtual void Free();
	};
}

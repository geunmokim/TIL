#include "..\default\framework.h"
#include "..\public\MainApp.h"

using namespace Client;

CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{

}

CMainApp* Client::CMainApp::Create()
{
	return nullptr;
}

void Client::CMainApp::Free()
{
	//__super::Free();

}

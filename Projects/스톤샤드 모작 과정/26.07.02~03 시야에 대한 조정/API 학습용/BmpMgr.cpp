#include "framework.h"
#include "BmpMgr.h"
#include "Bitmap.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;
CBmpMgr::CBmpMgr()
{

}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* _pFilePath, const TCHAR* _pImageKey)
{
	//같은 이름으로 등록된 이미지가 있는지 순회하면서 찾아라
	auto iter = find_if(m_mapBmp.begin(), m_mapBmp.end(), CStrCmp(_pImageKey)); //begin부터 end까지 하나씩 순회하면서 CStrCmp(_pImageKey)가 true를 반환하는 첫번쨰 원소를 찾으면  그위치를 주고,못찾으면 end
	//여기서 auto& 말고 auto를 사용한건 find_if의 특수성 떄문 //보통의 경우엔 auto&로 복사를 피하는게 맞음


	//중복 로딩 방지
	if (m_mapBmp.end() == iter)            //만약 find_if로 CstrCmp의 조건을 만족하는 원소를 못찾았다면 //같은 이름으로 등록된 이미지가 없다면
	{
		CMyBitmap* pBmp = new CMyBitmap;   //CMyBitmap의 객체를 새로 만들고
		pBmp->Load_Bmp(_pFilePath);        //정해진 경로에 있는 데이터를 불러와라
		m_mapBmp.emplace(_pImageKey, pBmp);//그리고 m_mapBmp라는 map에 _pImageKey라는 Key값과 pBmp라는 value값을 삽입해라

	}

}

HDC CBmpMgr::Find_Image(const TCHAR* _pImageKey)
{
	auto iter = find_if(m_mapBmp.begin(), m_mapBmp.end(), CStrCmp(_pImageKey));

	if (m_mapBmp.end() == iter)
		return nullptr;         //반환 타입이 HDC인데 못찾았다면 못찾았다고 nullptr 로 알려줌

	return iter->second->Get_DC();//같은 이름으로 등록된 이미지가 있다면 iter->second 즉, 맵의 key,value 중 second인 value값을 또 그 value가 가리키는 객체의 Get_DC()를 해라


}
void CBmpMgr::Release()
{
	for (auto& pBmp : m_mapBmp)
		SAFE_DELETE(pBmp.second);

	m_mapBmp.clear();

}
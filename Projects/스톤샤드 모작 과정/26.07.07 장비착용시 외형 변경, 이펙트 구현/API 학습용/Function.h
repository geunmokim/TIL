#pragma once
template <typename T>
void Safe_Delete(T& _Dst)
{
	if (_Dst)
	{
		delete _Dst;
		_Dst = nullptr;
	}
}

//y좌표 정렬을 위해 작성
template<typename T>
bool CompareY(T& _Dst, T& _Src)
{
	return _Dst->Get_Info().fY < _Src->Get_Info().fY;

}



//리스트의 정렬은 sort를 이용한다

#pragma once
namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;
	typedef wchar_t						_tchar;

	typedef XMFLOAT2					_float2;
	typedef XMFLOAT3					_float3;
	typedef XMFLOAT4					_float4;
	//XMFLOAT2,3,4 -> 저장용 타입
	//float 여러개를 묶어놓은 평범한 구조체
	//메모리에 값을 저장하는 용도로 사용

	

	/* SIMD */

	typedef XMVECTOR					_vector;


	typedef XMFLOAT4X4					_float4x4;

	typedef XMMATRIX					_matrix;
	//연산용 타입
	//실제 수학연산을 빠르게 수행ㅇ할떄 사용

	//저장용 타입과 연산용 타입으로 나눈 이유
	//->xmvector를 그냥 정점이나 구조체 멤버로 사용하면 메모리 정렬 요구사항 때문에 문제가 생기거나 메모리를 낭비하게된다
	//->저장할때(XMFLOAT3) ->연산할때(XMVECTOR 로드) -> 계산 -> 다시 저장(XMFLOAT3로 저장)식으로 용도로 분명하게 나눠서 해결

	/*
	 실사용 흐름 
     XMFLOAT3 pos1, pos2, result; // 저장용

     XMVECTOR v1 = XMLoadFloat3(&pos1);   // 저장용 → 연산용으로 "로드"
     XMVECTOR v2 = XMLoadFloat3(&pos2);
     XMVECTOR vResult = XMVectorAdd(v1, v2); // 연산은 XMVECTOR로 (SIMD로 빠르게)
     XMStoreFloat3(&result, vResult);      // 연산용 → 저장용으로 다시 "저장"
	
	*/
}
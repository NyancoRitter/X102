#pragma once

namespace Toyger
{
	/// <summary>ゲーム実装側が入力状態を得る手段</summary> 
	/// <typeparam name="UINT_T">キー押下状態履歴保持用の符号なし整数型（履歴データのbit数変更用）</typeparam>
	template< typename UINT_T = unsigned char >
	class IInputState
	{
	public:
		virtual ~IInputState(){}
	public:
		using KeyHistValType = UINT_T;
	public:
		/// <summary>マウスカーソル X 位置（クライアント領域座標）</summary>
		virtual int MouseX() const = 0;
		/// <summary>マウスカーソル Y 位置（クライアント領域座標）</summary>
		virtual int MouseY() const = 0;

		/// <summary>キーあるいはマウスボタンの押下状態履歴</summary>
		/// <param name="VKeyCode">対象のキーまたはマウスボタンの仮想キーコード</param>
		/// <returns>
		/// UINT_T 型のbit数分の状態履歴．
		/// bitが立っていれば「押されている/いた」ことを示す．
		/// 上位bit ほど古く，下位bitほど新しい．（時間と共に左シフトされていく）
		/// 	<remarks>例えば，履歴値の下位2bitが b'01 であれば「新しく押された」と判断できる．</remarks>
		/// </returns>
		virtual UINT_T KeyStateHist( unsigned char VKeyCode ) const = 0;

		/// <summary>ウィンドウを閉じようとする操作が成されたか否か</summary>
		/// <returns>
		/// ウィンドウを閉じる操作が成された（そしてその操作自体はその時点では握りつぶされた）場合にはtrueを返す．
		///		<remarks>入力状態データが更新された際に，このメソッドの戻り値はfalseにリセットされ得る．</remarks>
		///</returns>
		virtual bool IsWndCloseOperationOccur() const = 0;
	};

	//---
	//ヘルパ関数
	
	//キーあるいはマウスボタンが押されているか
	template< class UINT_T >
	inline bool Pressed( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return IS.KeyStateHist(VKeyCode) & UINT_T{0x01};	}

	//キーあるいはマウスボタンが押されていない状態から押されている状態になったか
	template< class UINT_T >
	inline bool PosEdge( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return ( (IS.KeyStateHist(VKeyCode) & UINT_T{0x03}) == UINT_T(0x01) );	}

	//キーあるいはマウスボタンが押されている状態から押されていない状態になったか
	template< class UINT_T >
	inline bool NegEdge( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return ( (IS.KeyStateHist(VKeyCode) & UINT_T{0x03}) == UINT_T(0x02) );	}
}

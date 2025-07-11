#pragma once
#include <string>

/// <summary>
/// 各シーンに提供される．
/// シーン切り替え等の手段を提供．
/// </summary>
class ITopLV
{
public:
	virtual ~ITopLV() = default;

public:
	//メッセージボックスのモーダル表示
	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const = 0;
};

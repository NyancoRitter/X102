#pragma once
#include "CampMenu.h"

namespace GameContent{	class PartyChar;	}

/// <summary>
/// キャンプメニューのページ
/// </summary>
class CampMenu::IPage : public IGUI
{
public:
	/// <summary>
	/// 選択されているキャラクタが変わった際の更新処理
	/// </summary>
	virtual void OnSelectedCharChanged() = 0;

	/// <summary>
	/// このページを操作対象にできるか否か
	/// </summary>
	/// <returns></returns>
	virtual bool CanEnter() const = 0;
};

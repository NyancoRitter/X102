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
	/// <param name="iCharOrder">選択されたキャラクタの位置</param>
	virtual void OnSelectedCharChanged( int iCharOrder ) = 0;

	/// <summary>
	/// このページを操作対象にできるか否か
	/// </summary>
	/// <returns></returns>
	virtual bool CanEnter() const = 0;

public:	// IGUI Impl
	virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装
};

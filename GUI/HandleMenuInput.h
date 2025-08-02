#pragma once

class IController;

namespace GUI::Menu
{
	class IMenuContent;

	//HandleInput()の戻り値用
	enum class HandleInputResult
	{
		None,	//有効な操作が行われなかった（何も変化無し）
		CursorMoved,	//カーソル位置が変化した
		Selected,	//項目選択操作が成された
		Canceled	//キャンセル操作が成された
	};

	/// <summary>
	/// メニュー用の一般的な操作入力処理
	/// </summary>
	/// <param name="TgtContent">操作対象</param>
	/// <param name="Controller">操作入力</param>
	/// <returns>処理結果</returns>
	HandleInputResult HandleInput( IMenuContent &TgtContent, const IController &Controller );
}

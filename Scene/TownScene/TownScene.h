#pragma once

#include "IScene.h"
#include "GUI/IGUI.h"

class ITopLV;
class PlayData;

namespace Town
{
	/// <summary>
	/// 街のシーン
	/// </summary>
	class TownScene : public IScene
	{
	public:
		TownScene( ITopLV &rTopLV );
		~TownScene();

	public:	// IScene Imple
		virtual void OnEnter() override;
		virtual void OnLeave() override;
		virtual Flags<SceneUpdateResult> Update( const IController &Controller ) override;
		virtual void Draw( HDC hdc ) override;

	private:
		class TownCenter_UI;
		class Inn_UI;
		class Pub_UI;
		class Shop_UI;

		PlayData &CurrPlayData();

		void Push_Inn_UI();
		void Push_Pub_UI();
		void Push_Shop_UI();
		void GoTo_Maze();

		//キャンプメニューを開く：引数スタックに積む．
		//ただし引数が nullptr である場合には，このオブジェクトが所有するスタックに積む．
		void Push_CampMenu_UI( GUI::GUIStack *pStack );

	public:
		ITopLV &m_rTopLV;
		GUI::GUIStack m_Stack;

		//※このUIは都度生成と破棄をせずに保持する
		std::unique_ptr< TownCenter_UI > m_upTownCenterUI;
	};
}

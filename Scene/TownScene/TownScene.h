#pragma once

#include "IScene.h"
#include "GUI/IGUI.h"

class ITopLV;
class PlayData;

namespace Town
{
	/// <summary>
	/// äXÇÃÉVÅ[Éì
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
		void Push_CampMenu_UI();
		void GoTo_Maze();

	public:
		ITopLV &m_rTopLV;
		GUI::GUIStack m_Stack;

		//Å¶Ç±ÇÃUIÇÕìsìxê∂ê¨Ç∆îjä¸ÇÇπÇ∏Ç…ï€éùÇ∑ÇÈ
		std::unique_ptr< TownCenter_UI > m_upTownCenterUI;
	};
}

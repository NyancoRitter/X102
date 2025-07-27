#pragma once

#include "IScene.h"
#include "GUI/IGUI.h"

class ITopLV;

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
		class INN_UI;
		class PUB_UI;
		class Shop_UI;

		void GoTo_TownCenter();
		void GoTo_Inn();
		void GoTo_Pub();
		void GoTo_Shop();
		void GoTo_Maze();

	public:
		ITopLV &m_rTopLV;
		GUI::GUIStack m_Stack;
	};
}

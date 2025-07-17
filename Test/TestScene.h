#pragma once
#include "IScene.h"
#include <memory>
#include "GUI/IGUI.h"

class ITopLV;
class CMonoBMP;


namespace GUI::Menu{	class Menu;	class IMenuContent;	}

/// <summary>
/// �e�X�g
/// </summary>
class TestScene : public IScene
{
public:
	TestScene( ITopLV &rTopLV );
	~TestScene();

public:	// IScene Impl
	virtual void OnEnter() override;
	//virtual void OnLeave() override;
	virtual Flags<SceneUpdateResult> Update( const IController &Controller ) override;
	virtual void Draw( HDC hdc ) override;

private:
	ITopLV &m_rTopLV;
	//std::unique_ptr<CMonoBMP> m_upMenuIcon;
	//std::unique_ptr< GUI::Menu::IMenuContent > m_upMenuContent;
	//std::unique_ptr< GUI::Menu::Menu > m_upMenu;
	GUI::GUIStack m_Stack;
};

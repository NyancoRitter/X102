#pragma once
#include "IScene.h"
#include <memory>

class ITopLV;
class CMonoBMP;

namespace TestSceneImpl
{
	class TestMenuContent;
}

/// <summary>
/// ƒeƒXƒg
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
	std::unique_ptr<CMonoBMP> m_upMenuIcon;
	std::unique_ptr<TestSceneImpl::TestMenuContent> m_upMenuContent;
};

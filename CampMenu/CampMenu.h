#pragma once

#include <functional>
#include "GUI/IGUI.h"
//#include "GUI/Menu.h"
//#include "GUI/MenuContent.h"
//#include "GUI/TextLinePainter.h"
//#include "GUI/ImgPainter.h"
#include "Common/MoneyView.h"
//#include "Common/CharSelMenuContent.h"

//class CMonoBMP;

class PlayData;
class TgtSpecifier;
namespace GameContent{	class Magic;	}

/// <summary>
/// �L�����v���j���[
/// �i�X�e�[�^�X�m�F�C�A�C�e���▂�@�̎g�p�����s����ʁj
/// </summary>
class CampMenu final : public GUI::IGUI
{
public:
	CampMenu( PlayData &rPlayData );
	~CampMenu();

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	//virtual void OnGotFocus() override;

	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
	virtual CampMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
	virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	class TopLVMenu;

	class IPage;
	class StatusPage;
	class ItemPage;
	class MagicPage;

	void OnTopLVMenuCursorMoved( int CharOrder, int CmdOrder );
	void OnTopLVMenuSelected( int CharOrder, int CmdOrder );

	/// <summary>
	/// �A�C�e���▂�@�̎g�p�ΏۃL�����N�^�I��pUI��UI�X�^�b�N�Ƀv�b�V��
	/// </summary>
	/// <param name="ForAll">�Ώۂ��S���̂��̂��ۂ�</param>
	/// <param name="Callback">�I��pUI�ɓn��Callback�i�d�l�� TgtSelection ���Q�� �j</param>
	void PushTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback );

	/// <summary>
	/// ���@�̎g�p����
	/// * �g�p�҂͈ÖٓI�Ɍ��݂���Ă���L�����N�^( m_iCurrChar )�ƂȂ�D
	/// * MP�I�Ɏg���邩�ۂ����̃`�F�b�N�������ōs����
	/// </summary>
	/// <param name="Magic">�g�p���閂�@</param>
	/// <param name="iTgtOrder">�g�p�ΏۃL�����N�^�̈ʒu�D���@���ʔ͈͂��P�̂ł���ꍇ�ɂ̂ݎg�p�����</param>
	void UseMagic( const GameContent::Magic &Magic, int iTgtOrder );

	////
	//PartyChar &ToChar( const TgtSpecifier &tgt );

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	MoneyView m_MoneyView;

	int m_iCurrChar = -1;
	int m_iCurrPage = -1;
	std::unique_ptr<TopLVMenu> m_upTopLVMenu;
	std::unique_ptr< IPage > m_Pages[3];

private:	//���C�A�E�g�p
	//�y�[�W�\���̈�̘g
	static const Rect MainAreaRect;

};


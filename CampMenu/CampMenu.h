#pragma once

#include "GUI/IGUI.h"
#include "Common/MoneyView.h"

#include "ActProc/ActEfficacy.h"
#include "ActProc/TgtRange.h"

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

	//MagicPage �Ŗ��@���I�����ꂽ�ۂ̏���
	void OnMagicSelected( const GameContent::Magic &Magic );

	/// <summary>
	/// ���@/�A�C�e�� ���ʂ̎g�p�����ʏ���
	/// </summary>
	/// <param name="Efficacy">����</param>
	/// <param name="Range">���ʔ͈�</param>
	/// <param name="iTgtOrder">�p�ΏۃL�����N�^�̈ʒu�DRange �������͈͂��P�̂ł���ꍇ�ɂ̂ݎg�p�����</param>
	/// <returns>�g�p�̐��ہD�g�p�s�����̂����p�ׂ��ꍇ�ɂ�false�D</returns>
	bool Affect( const ActEfficacy &Efficacy, TgtRange Range, int iTgtOrder );

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	MoneyView m_MoneyView;

	int m_iCurrChar = -1;
	int m_iCurrPage = -1;
	std::unique_ptr<TopLVMenu> m_upTopLVMenu;
	std::unique_ptr<StatusPage> m_upStatusPage;
	std::unique_ptr<ItemPage> m_upItemPage;
	std::unique_ptr<MagicPage> m_upMagicPage;
	IPage *m_Pages[3];

private:	//���C�A�E�g�p
	//�y�[�W�\���̈�̘g
	static const Rect MainAreaRect;

};


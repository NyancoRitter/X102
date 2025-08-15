#pragma once

#include "GUI/IGUI.h"
#include "Common/MoneyView.h"
#include "Common/EffectList.h"
#include "ActProc/ActEfficacy.h"
#include "ActProc/TgtRange.h"

//class CMonoBMP;

class PlayData;
class TgtSpecifier;
namespace GameContent
{
	class PartyChar;
	class Magic;
}

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
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	class TopLVMenu;

	class IPage;
	class StatusPage;
	class ItemPage;
	class MagicPage;

	/// <summary>���ݑI������Ă���L�����N�^�̃f�[�^�Q�Ɨp�D</summary>
	/// <returns>�Y���Ȃ��̏�Ԃł�nullptr��Ԃ�</returns>
	GameContent::PartyChar *CurrSelChar();

	//--- for TopLVMenu
	void OnTopLVMenuCursorMoved( int CharOrder, int CmdOrder );
	void OnTopLVMenuSelected( int CharOrder, int CmdOrder );

	//--- for MagicPage

	/// <summary>MagicPage �Ŗ��@���I�����ꂽ�ۂ̏���</summary>
	/// <param name="Magic">
	/// �I�����ꂽ���@�D
	/// �g�p�ł��Ȃ����@���w�肵���ꍇ�ɂ͉������Ȃ��D
	/// �i���̏�ʂŎg���Ȃ����@�ł���ꍇ��C���݂̃L�����N�^��MP�I�Ɏg���Ȃ��ꍇ���j
	/// </param>
	void OnMagicSelected( const GameContent::Magic &Magic );

	//--- for ItemPage

	/// <summary>ItemPage �ŃA�C�e�����I�����ꂽ�ۂ̏���</summary>
	/// <param name="iOrder">
	/// �I�����ꂽ�A�C�e���́i���݂̃L�����N�^�̎��������ł́jindex�D
	/// �͈͊O���̗L���łȂ��l��n�����ꍇ�ɂ͉������Ȃ��D
	/// </param>
	void OnItemSelected( int iOrder );

	/// <summary>�i�����������p�����j�����Ŏw�肳�ꂽ�L�����N�^�����i�̈ʒu�𖖔��ɉ�</summary>
	/// <param name="iOrder">�ΏۃA�C�e���́i���݂̃L�����N�^�̎��������ł́jindex</param>
	/// <returns>���ہD�͈͊O���̗L���łȂ��l���w�肵���ꍇ�ɂ͉���������false��Ԃ�</returns>
	bool MoveBackCharItem( int iOrder );

	/// <summary>�i�����������p�����j�����Ŏw�肳�ꂽStock���A�C�e���̈ʒu�𖖔��ɉ�</summary>
	/// <param name="iOrder">�ΏۃA�C�e���́iStock���ł́jindex</param>
	/// <returns>���ہD�͈͊O���̗L���łȂ��l���w�肵���ꍇ�ɂ͉���������false��Ԃ�</returns>
	bool MoveBackStockItem( int iOrder );

	/// <summary>�L�����N�^�̏����i��Stock�Ɉړ�����</summary>
	///  <param name="iOrder">�ΏۃA�C�e���́i���݂̃L�����N�^�̎��������ł́jindex</param>
	/// <returns>���ہD�͈͊O���̗L���łȂ��l���w�肵���ꍇ�ɂ͉���������false��Ԃ�</returns>
	bool MoveItemFromCharToStock( int iOrder );

	/// <summary>Stock���̃A�C�e�����L�����N�^�����i�Ɉړ�����</summary>
	///  <param name="iOrder">�ΏۃA�C�e���́iStock���ł́jindex</param>
	/// <returns>���ہD�͈͊O���̗L���łȂ��l���w�肵���ꍇ�ɂ͉���������false��Ԃ�</returns>
	bool MoveItemFromStockToChar( int iOrder );

private:
	/// <summary>
	/// �A�C�e���▂�@�̎g�p�ΏۃL�����N�^�I��pUI��UI�X�^�b�N�Ƀv�b�V��
	/// </summary>
	/// <param name="ForAll">�Ώۂ��S���̂��̂��ۂ�</param>
	/// <param name="Callback">�I��pUI�ɓn��Callback�i�d�l�� TgtSelection ���Q�� �j</param>
	void PushTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback );

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

	EffectList m_EffectList;

private:	//���C�A�E�g�p
	//�y�[�W�\���̈�̘g
	static const Rect MainAreaRect;

};


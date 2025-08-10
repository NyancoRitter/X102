#pragma once
#include "Toyger/Toyger_InputImpl.h"
#include "Toyger/Toyger_IContentPainter.h"
#include <memory>
#include "ITopLV.h"

namespace Toyger{ class IWnd; }
using InputState = Toyger::IInputState<uint8_t>;

class IScene;
namespace Town{	class TownScene;	}

class PlayData;

/// <summary>
/// �Q�[������
/// </summary>
class Game final
	: public Toyger::IContentPainter
	, public ITopLV
{
public:
	/// <summary>
	/// ctor. �E�B���h�E��DI
	/// </summary>
	/// <param name="Wnd">
	/// �����ς̃E�B���h�E���Ǘ����Ă���C���X�^���X�ւ̎Q�ƁD
	/// ���̎Q�Ɛ�̃C���X�^���X�̎��������łȂ��C
	/// �E�B���h�E�̗L�����Ԃ��C����Game�C���X�^���X��������
	/// �i�F�v�́C����Game�C���X�^���X�̃��\�b�h���Ă΂��Ƃ��C�E�B���h�E�͕����Ă��Ȃ��j�Ƃ����z��D
	/// </param>
	Game( Toyger::IWnd &Wnd );
	~Game();

public:
	/// <summary>
	/// ���������D
	/// �v���O�����N����`���C�����[�v�܂ł̊ԂɂP�x�����Ă΂��z��D
	/// �i�Q��ȏ�Ăяo�����ۂ̓���͖��ۏ�j
	/// </summary>
	/// <returns>���ہD���s�s�\�Ȏ��s���������ۂ�false</returns>
	bool Initialize();

	/// <summary>
	/// �X�V�����D
	/// ���C�����[�v����Ă΂��z��D
	///		<remarks>
	///		���O�� Initialize() ���P�x�����Ă΂�C
	///		���C���̍ہC Initialize() �� true ��Ԃ��Ă���K�v������D
	///		���̎��O�����𖞂������ɂ��̃��\�b�h���Ă񂾍ۂ̓���͖��ۏ�D
	/// 
	///		�܂��C���̃��\�b�h�� false ��Ԃ��Ĉȍ~�́C
	///		���̃��\�b�h�̓���͖��ۏ�D
	///		</remarks>
	/// </summary>
	/// <param name="Input">���͏��</param>
	/// <returns>
	/// ���s or Not�D
	/// false�̓v���O�������I��������ׂ����Ƃ������D
	/// </returns>
	bool Update( const InputState &Input );

public:	// Toyger::IContentPainter Impl
	virtual void Paint( HDC hdc, int W, int H ) override;

public:	// ITopLV Imp;
	virtual PlayData &CurrPlayData() override;
	virtual void ChangeToTownScene() override;
	
	virtual bool ChangeBGM( int BGM_index ) override;
	virtual void StopBMG() override;

	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const override;

private:
	//���֌W�̏���
	void SetupSound();

	//�������w���V�[�����J�����g�V�[���ɂ���
	void ChangeCurrSceneTo( IScene *pScene );

	//�\���{����ύX
	void ChangeViewMagRate( int Rate );

private:
	Toyger::IWnd &m_rWnd;	//�E�B���h�E�ւ̃A�N�Z�X��i�Dctor�Ŏw�肳�ꂽ�Q�Ƃ�ێ�
	HFONT m_hDefaultFont = NULL;	//�f�t�H���g�̃t�H���g

	//
	std::unique_ptr< PlayData > m_upPlayData;	//�v���C�󋵃f�[�^

	//
	IScene *m_pCurrScene = nullptr;	//�J�����g�V�[��
	std::unique_ptr< Town::TownScene > m_upTownScene;	//�e�X�g�p
};

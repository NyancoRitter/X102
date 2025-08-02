#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GUI/ImgPainter.h"

namespace ResManage{	class MonoBmpHolder;	}

namespace Town
{
	/// <summary>
	/// �X�̒��SUI
	/// 
	/// �s������j���[�őI�����邾���̉��
	/// </summary>
	class TownScene::TownCenter_UI final : public GUI::IGUI
	{
	public:
		TownCenter_UI( TownScene &Outer );
		~TownCenter_UI();

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;

		virtual void OnGotFocus() override {	this->Visible(true);	}
		virtual void OnLostFocus() override {	this->Visible(false);	}

		virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
		virtual TownCenter_UI &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
		virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����

	protected:
		virtual void Paint_( HDC hdc ) const override;

	public:
		/// <summary>
		/// �w�i�摜���C�ʏ�̊X�̊G�ł͂Ȃ��u���͂���v�ɂ���D
		/// �������C���̕ύX�͎{�݁i�X�Ȃǁj�ւ̈ړ��������������_�Ŏ����I�Ƀ��Z�b�g�����D
		///		<remarks>
		///		���p�[�e�B���퓬�s�k������̕\���p
		///		</remarks>
		/// </summary>
		void SetImgToOutskirts();

	private:
		TownScene &m_Outer;

		//�\����
		GUI::TextLinePainter m_Header;
		GUI::MonoImgPainter m_ImgPainter;
		//���j���[
		GUI::Menu::Menu m_Menu;
		GUI::Menu::GenericMenuContent<true> m_MenuContent;
	};
}

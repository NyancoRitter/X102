#pragma once

#include "IScene.h"
#include "GUI/IGUI.h"

class ITopLV;
class PlayData;

namespace Town
{
	/// <summary>
	/// �X�̃V�[��
	/// </summary>
	class TownScene : public ITownScene
	{
	public:
		TownScene( ITopLV &rTopLV );
		~TownScene();

	public:	// ITownScene Imple
		virtual void OnEnter() override;
		virtual void OnLeave() override;
		virtual Flags<SceneUpdateResult> Update( const IController &Controller ) override;
		virtual void Draw( HDC hdc ) override;
		virtual void SetImgToOutskirts() override;

	private:
		class TownCenter_UI;
		class Inn_UI;
		class Pub_UI;
		class Shop_UI;

		PlayData &CurrPlayData();

		void Push_Inn_UI();
		void Push_Pub_UI();
		void Push_Shop_UI();
		void GoTo_Maze();	//���H�V�[���ւ̑J�ځD

		//�L�����v���j���[���J���F�����X�^�b�N�ɐςށD
		//������������ nullptr �ł���ꍇ�ɂ́C���̃I�u�W�F�N�g�����L����X�^�b�N�ɐςށD
		void Push_CampMenu_UI( GUI::GUIStack *pStack );

	public:
		ITopLV &m_rTopLV;
		GUI::GUIStack m_Stack;

		//������UI�͓s�x�����Ɣj���������ɕێ�����
		std::unique_ptr< TownCenter_UI > m_upTownCenterUI;
	};
}

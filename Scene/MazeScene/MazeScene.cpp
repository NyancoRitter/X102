#include "framework.h"
#include "MazeScene.h"
#include "ITopLV.h"

#include <sstream>

#include "IController.h"
#include "GlobalConst.h"
#include "Rnd.h"

#include "PlayData/PlayData.h"
#include "DataPath.h"

#include "GUI/DrawFuncs.h"
#include "GUI/SimpleSelection.h"

#include "CampMenu/CampMenu.h"
#include "MazeDataImpl/LoadMazeDataFile.h"
#include "MoveEffect.h"
#include "MazeTitleEffect.h"

using namespace GUI;
using namespace MazeDataImpl;


std::unique_ptr<IScene> CreateMazeScene( ITopLV &rTopLV )
{	return std::make_unique< Maze::MazeScene >( rTopLV, DataPath::MazeMapFilePathU8() );	}

namespace
{
	//�S�����P�ʃx�N�g��
	constexpr Vec2i UnitVecs[4] = { {1,0}, {0,1}, {-1,0}, {0,-1} };
	inline const Vec2i &UnitVec( Direction Dir ){	return UnitVecs[(int)Dir];	}
};

namespace Maze
{
	MazeScene::MazeScene( ITopLV &rTopLV, const std::string &MazeFilePathU8 )
		: m_rTopLV( rTopLV )
		, m_Renderer( GlobalConst::GC_W, GlobalConst::GC_H, PI*0.55, 3.2 )
		, m_UsualUpdater( *this )
	{
		//�t�@�C��������H�f�[�^��Load�i���s���͗�O���o�j
		m_MazeMap = LoadMazeMapFile( MazeFilePathU8, m_StartPos, m_StartDir );
	}

	//
	void MazeScene::OnEnter()
	{
		m_CurrFloor = 0;
		m_CurrPos = m_StartPos;
		m_CurrDir = m_StartDir;
		
		m_Renderer.ResetCamera( m_StartPos, m_StartDir );

		m_bShowPosInfo = false;
		m_Renderer.SetBrightnessRate( 0 );
		m_CmdSeq.clear();
		m_CmdSeq.emplace_back( TitleEffect( m_EffectsPainter, ms_nAnimFrame_Ladder ) );
		m_CmdSeq.emplace_back( LadderEffect::DownFromCeil( m_Renderer, ms_nAnimFrame_Ladder ) );
		m_CmdSeq.emplace_back( [this]()->Flags<CmdResult>{	m_bShowPosInfo=true;	return CmdResult::Finished|CmdResult::ReqRedraw;	}	);

		m_Stack.clear( false );
		m_Stack.Push( std::make_unique<RefWrapper>( m_UsualUpdater ) );
	}

	void MazeScene::OnLeave()
	{
		m_Stack.clear( false );
	}

	//Update
	Flags<SceneUpdateResult> MazeScene::Update( const IController &Controller )
	{
		Flags<SceneUpdateResult> Ret;

		//CmdSeq
		if( !m_CmdSeq.empty() )
		{
			const auto result = UpdateCmdSeq( m_CmdSeq );
			if( result.Has( CmdResult::ReqRedraw ) ){	Ret |= SceneUpdateResult::ReqRedraw;	}
			if( result.Has( CmdResult::SuppressSubsequents ) ){	return Ret;	}
		}

		//���͂�����
		if( m_Stack.Update( Controller ) )
		{	Ret |= SceneUpdateResult::ReqRedraw;	}

		return Ret;
	}

	//�ʏ�̓��͏���
	Flags<GUI::GUIResult> MazeScene::UsualUpdate( const IController &Controller )
	{
		//�ړ��C����
		if( Controller.MoveFront() ){	return ( BeginWalk( m_CurrDir )  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		if( Controller.MoveBack() ){	return ( BeginWalk( OppositeDirOf(m_CurrDir) )  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		if( Controller.TurnRight() ){	BeginTurn( true );	return GUIResult::ReqRedraw;	}
		if( Controller.TurnLeft() ){	BeginTurn( false );	return GUIResult::ReqRedraw;	}
		//�u���ׂ�v
		if( Controller.Inspect() ){	return ( Inspect()  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		//�p�[�e�B���j���[
		if( Controller.OpenCampMenu() ){	OpenCampMenu();	return GUIResult::ReqRedraw;	}


#ifdef _DEBUG	//For Test
		if( Controller.TestKey() )
		{
			m_TestModeFlag = !m_TestModeFlag;
			return GUIResult::ReqRedraw;
		}
#endif

		return GUIResult::None;
	}

	//�`��
	void MazeScene::Draw( HDC hdc )
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		m_Stack.Paint( hdc );
		m_EffectsPainter.Paint( hdc );
	}

	//�ʏ펞�̖��H�`��
	void MazeScene::UsualPaint( HDC hdc )
	{
		m_Renderer.Draw( hdc, m_MazeMap[m_CurrFloor] );

		if( m_bShowPosInfo )
		{	DrawPosInfo( hdc, { 32, GlobalConst::GC_H-24, GlobalConst::GC_W, GlobalConst::GC_H } );	}

#ifdef _DEBUG
		if( m_TestModeFlag )
		{
			::SetTextColor( hdc, RGB(255,255,255) );
			::SetBkMode( hdc, TRANSPARENT );
			RECT reg{ 0,0, 100,100 };
			::DrawTextW( hdc, L"(TestMode)", -1, &reg, DT_NOCLIP|DT_SINGLELINE|DT_TOP|DT_LEFT );
		}
#endif
	}

	//�ʒu���̕`��
	void MazeScene::DrawPosInfo( HDC hdc, const RECT &Rect )
	{
		constexpr wchar_t DirChar[] = L"ESWN";
		std::wstringstream SS;
		SS << L"B" << m_CurrFloor+1
			<< L" (" << m_CurrPos[0] << L"," << m_CurrPos[1] << L")  [" << DirChar[(int)m_CurrDir] << L"]";
		
		::SetTextColor( hdc, RGB(255,255,255) );
		::SetBkMode( hdc, TRANSPARENT );
		RECT reg = Rect;
		::DrawTextW( hdc, SS.str().c_str(), -1, &reg, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
	}

	//�p�[�e�B���j���[���J��
	void MazeScene::OpenCampMenu(){	m_Stack.Push( std::make_unique<CampMenu>( m_rTopLV.CurrPlayData() ) );	}

	//�퓬�J�n
	void MazeScene::Encount( const std::vector<unsigned int> &EnemyIDs, bool IsEscapableBattle )
	{
		////�G���J�E���g���G�t�F�N�g�@���@�퓬�V�[���̊J�n
		//m_EffectList.Push_Back( CreateEncountEffect() );
		//m_EffectList.Push_Back(
		//	std::make_unique< UI::SimpleTask >(
		//		[this, IDs=EnemyIDs, IsEscapableBattle]()->bool
		//		{
		//			m_UI.Add( CreateBattleScene( m_rGameTopLV.CurrPlayData(), IDs, IsEscapableBattle, m_ProcAfterBattle ) );
		//			return false;
		//		}
		//	)
		//);
	}

	//�����_���G���J�E���g����
	void MazeScene::RandomEncount()
	{
#ifdef _DEBUG
		if( m_TestModeFlag )return;
#endif
		//if( RND().GetInt<int>( 0, 99 ) >= m_EncountPercentage )return;
		//m_EncountPercentage /= 7;

		//constexpr int BGMs[3] = { BGMSpecifier::Battle1, BGMSpecifier::Battle2, BGMSpecifier::Battle3 };
		//m_rGameTopLV.ChangeBGM( BGMs[ RND().GetInt<int>(0,2) ] );

		//Encount( GameContent::Battle::CreateRndEnemyParty(m_CurrFloor), true );
	}

	//�w������Ɉ���i�ޏ����̊J�n�D�ړ��J�n���������{�����ꍇ�ɂ� true ��Ԃ��D
	//�������C������Ɉړ��ł��Ȃ��ꍇ�ɂ͉��������� false ��Ԃ��D
	bool MazeScene::BeginWalk( Direction Dir )
	{
		Vec2i To = m_CurrPos + UnitVec(Dir);
#ifdef _DEBUG
		if( !m_TestModeFlag )
#endif
		{//�ړ��ۃ`�F�b�N
			auto Edge = m_MazeMap[ m_CurrFloor ].GetEdgeAttr( To, OppositeDirOf(Dir) );
			if( Edge == (int)EdgeAttr::WALL )return false;
			if( Edge >= (int)EdgeAttr::DOOR )
			{
				int DoorLockLV = Edge - (int)EdgeAttr::DOOR;
				if( !m_rTopLV.CurrPlayData().CanMoveThrough( DoorLockLV ) )return false;
			}
		}

		m_CmdSeq.emplace_back( WalkEffect( m_Renderer, m_CurrPos, To, ms_nAnimFrame_Walk ) );
		m_CmdSeq.emplace_back(
			[this,To]()->Flags<CmdResult>
			{
				m_CurrPos = To;
				m_Renderer.ResetCamera( m_CurrPos, m_CurrDir );

				m_EncountPercentage += RND().GetInt( 0,5 );

				//TODO : �C�x���g�`�F�b�N�D�C�x���g�������ꍇ�ɂ̓����_���G���J�E���g����

				return CmdResult::Finished | CmdResult::ReqRedraw;
			}
		);

		return true;
	}

	//���E�ǂ��炩��90�x���񂷂鏈���̊J�n�D
	void MazeScene::BeginTurn( bool ToRight )
	{
		m_CmdSeq.emplace_back( TurnEffect( m_Renderer, m_CurrDir, ToRight, ms_nAnimFrame_Turn ) );
		m_CmdSeq.emplace_back(
			[this,ToRight]()->Flags<CmdResult>
			{
				m_CurrDir = ( ToRight ? RightDirOf(m_CurrDir) : LeftDirOf(m_CurrDir) );
				m_Renderer.ResetCamera( m_CurrPos, m_CurrDir );
				return CmdResult::Finished | CmdResult::ReqRedraw;
			}
		);
	}

	//�u���ׂ�v����
	//�����������J�n���ꂽ���ۂ���Ԃ�
	bool MazeScene::Inspect()
	{
		//TODO : �C�x���g����


		//��q����
		const auto &CurrFloor = m_MazeMap[ m_CurrFloor ];
		const auto Obj = CurrFloor.GetObjectAt( m_CurrPos );
		if( !Obj )return false;

		if( Obj.value().Type == ObjType::UP_STAIR )
		{//����
			if( m_CurrFloor==0 )
			{//���{����o���q�ł���ꍇ
				auto upConfirmUI = std::make_unique< Menu::SimpleSelection >(
					L"�� ���{�̏o�� ��",
					Vec2i{128,32}
				);
				upConfirmUI->Width(320).Add( L"���ɖ߂�" ).Add( L"���܂�" )
					.OnSelect(
						[this](int index)->bool{
							if( index==0 )
							{
								m_CmdSeq.emplace_back( LadderEffect::UpToCeil( m_Renderer, ms_nAnimFrame_Ladder) );
								m_CmdSeq.emplace_back(
									[this]()->Flags<CmdResult>
									{	m_rTopLV.ChangeToTownScene();	return CmdResult::Finished|CmdResult::SuppressSubsequents|CmdResult::ReqRedraw;	}
								);
							}
							return true;
						}
					)
					.XCenter( GlobalConst::GC_W ).YCenter( GlobalConst::GC_H );
				m_Stack.Push( std::move(upConfirmUI) );
				return true;
			}
			else
			{//��K��
				m_CmdSeq.emplace_back( LadderEffect::UpToCeil( m_Renderer, ms_nAnimFrame_Ladder) );
				m_CmdSeq.emplace_back(
					[this]()->Flags<CmdResult>{	--m_CurrFloor;	return CmdResult::Finished|CmdResult::ReqRedraw;	}
				);
				m_CmdSeq.emplace_back( LadderEffect::UpFromFloor( m_Renderer, ms_nAnimFrame_Ladder) );

				m_EncountPercentage = 0;
				return true;
			}
		}
		else if( Obj.value().Type == ObjType::DOWN_STAIR )
		{//�~��
			if( m_CurrFloor+1 < (int)m_MazeMap.size() )
			{//���K��
				m_CmdSeq.emplace_back( LadderEffect::DownToFloor( m_Renderer, ms_nAnimFrame_Ladder) );
				m_CmdSeq.emplace_back(
					[this]()->Flags<CmdResult>{	++m_CurrFloor;	return CmdResult::Finished|CmdResult::ReqRedraw;	}
				);
				m_CmdSeq.emplace_back( LadderEffect::DownFromCeil( m_Renderer, ms_nAnimFrame_Ladder) );

				m_EncountPercentage = 0;
				return true;
			}
			else
			{//�����w�f�[�^�������ꍇ�ւ̔���
				m_rTopLV.ShowMsgBox( L"ERR", L"�t���A�f�[�^���s�����Ă���" );
				return false;
			}
		}

		return false;
	}

}
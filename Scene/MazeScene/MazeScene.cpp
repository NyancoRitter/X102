#include "framework.h"
#include "MazeScene.h"
#include "ITopLV.h"

#include <sstream>
//#include "CreateMazeScene.h"

#include "IController.h"
#include "GlobalConst.h"
//#include "Rnd.h"

#include "PlayData/PlayData.h"
#include "DataPath.h"

#include "GUI/DrawFuncs.h"
#include "GUI/SimpleSelection.h"

#include "CampMenu/CampMenu.h"
#include "MazeDataImpl/LoadMazeDataFile.h"
#include "MoveEffect.h"

using namespace GUI;
using namespace MazeDataImpl;


std::unique_ptr<IScene> CreateMazeScene( ITopLV &rTopLV )
{	return std::make_unique< Maze::MazeScene >( rTopLV, DataPath::MazeMapFilePathU8() );	}

namespace
{
	//４方向単位ベクトル
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
		//ファイルから迷路データをLoad（失敗時は例外送出）
		m_MazeMap = LoadMazeMapFile( MazeFilePathU8, m_StartPos, m_StartDir );

		m_upWalkEffect = std::make_unique<WalkEffect>( m_Renderer );
		m_upTurnEffect = std::make_unique<TurnEffect>( m_Renderer );
	}

	//
	void MazeScene::OnEnter()
	{
		m_CurrFloor = 0;
		m_CurrPos = m_StartPos;
		m_CurrDir = m_StartDir;
		m_ShouldBackToTown = false;
		
		m_Renderer.ResetCamera( m_StartPos, m_StartDir );
		m_Renderer.SetBrightnessRate( 0 );

		
		//m_bShowPosInfo = false;

		m_EffectList.clear();
		//{
		//	auto spPainter = std::make_shared< UI::FramedTextPainter >( GC_W-4, 40 );
		//	spPainter->Text( u8"=== ENTERING THE MAZE OF PALMETINA ===" ).TopLeft( {0,2} ).XCenter( GC_W );
		//	constexpr int Wait = 12;
		//	m_EffectList.Push_Front( std::make_shared<UI::DispForCertainPeriod>( spPainter, ms_nAnimFrame_Ladder+Wait, Wait ) );
		//}
		m_EffectList.PushBack( LadderEffect::DownFromCeil( m_Renderer, ms_nAnimFrame_Ladder ) );
		//m_EffectList.PushBack( std::make_unique<UI::SimpleTask>( [&f=m_bShowPosInfo]()->bool{	f=true;	return false;	} ) );

		m_Stack.clear( false );
		m_Stack.Push( std::make_unique<RefWrapper>( m_UsualUpdater ) );
	}

	void MazeScene::OnLeave()
	{
		m_EffectList.clear();
		m_Stack.clear( false );
	}

	//Update
	Flags<SceneUpdateResult> MazeScene::Update( const IController &Controller )
	{
		//bool NeedToRedraw = false;

		//エフェクト処理
		if( !m_EffectList.empty() )
		{
			m_EffectList.Update();
			return SceneUpdateResult::ReqRedraw;
		}

		//町に戻る処理
		if( m_ShouldBackToTown )
		{
			m_rTopLV.ChangeToTownScene();
			return SceneUpdateResult::ReqRedraw;
		}

		//入力を処理
		if( m_Stack.Update( Controller ) )
		{	return SceneUpdateResult::ReqRedraw;	}

		return SceneUpdateResult::None;
	}

	//通常の入力処理
	Flags<GUI::GUIResult> MazeScene::UsualUpdate( const IController &Controller )
	{
		//移動，旋回
		if( Controller.MoveFront() ){	return ( BeginWalk( m_CurrDir )  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		if( Controller.MoveBack() ){	return ( BeginWalk( OppositeDirOf(m_CurrDir) )  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		if( Controller.TurnRight() ){	BeginTurn( true );	return GUIResult::ReqRedraw;	}
		if( Controller.TurnLeft() ){	BeginTurn( false );	return GUIResult::ReqRedraw;	}
		//「調べる」
		if( Controller.Inspect() ){	return ( Inspect()  ?  GUIResult::ReqRedraw  :  GUIResult::None );	}
		//パーティメニュー
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

	//Paint
	void MazeScene::Draw( HDC hdc )
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		m_Stack.Paint( hdc );
		//m_EffectList.Paint(hdc);
	}

	//迷路の描画
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

	//位置情報の描画
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

	//パーティメニューを開く
	void MazeScene::OpenCampMenu(){	m_Stack.Push( std::make_unique<CampMenu>( m_rTopLV.CurrPlayData() ) );	}

	//戦闘開始
	void MazeScene::Encount( const std::vector<unsigned int> &EnemyIDs, bool IsEscapableBattle )
	{
		////エンカウント時エフェクト　→　戦闘シーンの開始
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

	//ランダムエンカウント処理
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

	//指定方向に一歩進む処理の開始．移動開始処理を実施した場合には true を返す．
	//ただし，そちらに移動できない場合には何もせずに false を返す．
	bool MazeScene::BeginWalk( Direction Dir )
	{
		Vec2i To = m_CurrPos + UnitVec(Dir);
#ifdef _DEBUG
		if( !m_TestModeFlag )
#endif
		{
			auto Edge = m_MazeMap[ m_CurrFloor ].GetEdgeAttr( To, OppositeDirOf(Dir) );
			if( Edge == (int)EdgeAttr::WALL )return false;
			if( Edge >= (int)EdgeAttr::DOOR )
			{
				int DoorLockLV = Edge - (int)EdgeAttr::DOOR;
				if( !m_rTopLV.CurrPlayData().CanMoveThrough( DoorLockLV ) )return false;
			}
		}

		m_CurrPos = To;
		m_Renderer.ResetCamera( m_CurrPos, m_CurrDir );
		return true;

		//m_WalkEffect.Reset(
		//	m_CurrPos, To, ms_nAnimFrame_Walk,
		//	[this](Vec2i pos)
		//	{
		//		m_CurrPos=pos;
		//		m_Renderer.ResetCamera( pos, m_CurrDir );
		//	}
		//);

		//m_EffectList.Push_Back( &m_WalkEffect );

		//m_EncountPercentage += RND().GetInt<int>( 0,5 );
		//m_EffectList.Push_Back(
		//	std::make_unique<UI::SimpleTask>(
		//		[this]()->bool
		//		{
		//			if( !StartEventProc( Event::EventTrigger::OnEnter ) )
		//			{	RandomEncount();	}
		//			return false;
		//		}
		//	) 
		//);
	}

	//左右どちらかに90度旋回する処理の開始．
	void MazeScene::BeginTurn( bool ToRight )
	{
		m_CurrDir = ( ToRight ? RightDirOf(m_CurrDir) : LeftDirOf(m_CurrDir) );
		m_Renderer.ResetCamera( m_CurrPos, m_CurrDir );

		//m_TurnEffect.Reset(
		//	m_CurrDir, ToRight, ms_nAnimFrame_Turn,
		//	[this](Direction dir){	m_CurrDir=dir;	m_Renderer.ResetCamera( m_CurrPos, dir );	}
		//);

		//m_EffectList.Push_Back( &m_TurnEffect );


	}

	//「調べる」処理
	//何か処理が開始されたか否かを返す
	bool MazeScene::Inspect()
	{
		//using namespace UI::Menu;

		////イベント処理
		//if( StartEventProc( Event::EventTrigger::OnInspect ) )
		//{	return true;	}

		//梯子処理
		const auto &CurrFloor = m_MazeMap[ m_CurrFloor ];
		const auto Obj = CurrFloor.GetObjectAt( m_CurrPos );
		if( !Obj )return false;

		if( Obj.value().Type == ObjType::UP_STAIR )
		{//昇り
			if( m_CurrFloor==0 )
			{//迷宮から出る梯子である場合
				auto upConfirmUI = std::make_unique< Menu::SimpleSelection >(
					L"＝ 迷宮の出口 ＝",
					Vec2i{128,32}
				);
				upConfirmUI->Width(320).Add( L"町に戻る" ).Add( L"留まる" )
					.OnSelect(
						[this](int index)->bool{
							if( index==0 )
							{
								//m_EffectList.Push_Back( LadderEffect::UpToCeil( m_Renderer, ms_nAnimFrame_Ladder) );
								m_ShouldBackToTown = true;
							}
							return true;
						}
					)
					.XCenter( GlobalConst::GC_W ).YCenter( GlobalConst::GC_H );
				m_Stack.Push( std::move(upConfirmUI) );
				return true;
			}
			else
			{//上階へ
				/*m_EffectList.Push_Back(
					std::make_unique< ChangeFloorEffect >( m_Renderer, m_CurrFloor, true, ms_nAnimFrame_Ladder, [this](int floor){	m_CurrFloor=floor;	} )
				);*/
				//m_EncountPercentage = 0;

				--m_CurrFloor;
				return true;
			}
		}
		else if( Obj.value().Type == ObjType::DOWN_STAIR )
		{//降り
			if( m_CurrFloor+1 < (int)m_MazeMap.size() )
			{//下階へ
				//m_EffectList.Push_Back(
				//	std::make_unique< ChangeFloorEffect >( m_Renderer, m_CurrFloor, false, ms_nAnimFrame_Ladder, [this](int floor){	m_CurrFloor=floor;	} )
				//);
				//m_rGameTopLV.CurrPlayData().OnReachedToFloor( m_CurrFloor+1 );
				//m_EncountPercentage = 0;
				++m_CurrFloor;
				return true;
			}
			else
			{//※下層データが無い場合への備え
				//OpenMsgWnd( { u8"穴の中は完全に土砂で埋没している。", u8"ここから下層には進めそうにない。" } );
				//return true;
				return false;
			}
		}

		return false;
	}

}
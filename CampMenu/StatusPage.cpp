#include "framework.h"
#include "StatusPage.h"
#include <sstream>

#include "PlayData/PlayData.h"
#include "GameContent/PartyChar.h"
#include "GameContent/Spell.h"
#include "ResManage/BmpBank.h"
#include "Parts/CMonoBmp.h"
#include "Common/CharDrawColor.h"

void CampMenu::StatusPage::Paint_( HDC hdc ) const
{
	using namespace ResManage;
	using namespace GameContent;
	
	const auto *pChar = m_Outer.CurrSelChar();
	if( !pChar )return;

	const auto TL = CampMenu::MainAreaRect.TopLeft() + Vec2i{ 16, 32 };
	constexpr int LineH = 24;
	constexpr int ImgStretchRate = 2;

	const auto ID = pChar->ID();

	const auto &FaceImg = FaceBMP( ID );
	FaceImg.StretchBlt( hdc, TL[0], TL[1], ImgStretchRate, ImgStretchRate, CharDrawColor( pChar->HP(), pChar->PoisonInfected() ) );

	SetTextColor( hdc, GUI::Color::White );
	SetBkMode( hdc, TRANSPARENT );

	{//Name, LV, BadStatus
		RECT rect;
		rect.left = TL[0];
		rect.right = rect.left + FaceImg.GetW()*ImgStretchRate;
		rect.top = TL[1] + FaceImg.GetH()*ImgStretchRate + 12;
		rect.bottom = rect.top + LineH;
		//Name
		DrawText( hdc, NameStr(ID).c_str(), -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		//LV
		OffsetRect( &rect, 0, LineH );
		DrawText( hdc, L"LV", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		DrawText( hdc, std::to_wstring(pChar->LV()).c_str(), -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
		//BadStatus
		if( pChar->PoisonInfected() )
		{
			OffsetRect( &rect, 0, LineH );
			SetTextColor( hdc, CharColor_PoisonInfected );
			DrawText( hdc, L"��", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		}
	}

	//
	RECT StatusRect;
	StatusRect.left = TL[0] + 40 + FaceImg.GetW()*ImgStretchRate;
	StatusRect.right = StatusRect.left + 128;
	StatusRect.top = TL[1];
	StatusRect.bottom = StatusRect.top + LineH;

	SetTextColor( hdc, GUI::Color::White );
	{//HP
		DrawText( hdc, L"HP", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		std::wstringstream SS;
		SS << pChar->HP() << L" / " << pChar->MaxHP();
		DrawText( hdc, SS.str().c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );	
	}
	{//STR, MAG, AGI
		OffsetRect( &StatusRect, 0, LineH + 12 );
		DrawText( hdc, L"STR", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		DrawText( hdc, std::to_wstring(pChar->STR()).c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
		OffsetRect( &StatusRect, 0, LineH );
		DrawText( hdc, L"MAG", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		DrawText( hdc, std::to_wstring(pChar->MAG()).c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
		OffsetRect( &StatusRect, 0, LineH );
		DrawText( hdc, L"AGI", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
		DrawText( hdc, std::to_wstring(pChar->AGI()).c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
	}

	{//�����ϐ�
		OffsetRect( &StatusRect, 0, LineH + 12 );
		DrawText( hdc, L"�����ϐ� :", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );

		constexpr int IconSize = 16;
		constexpr AttackElement Elems[4] = { AttackElement::Physical, AttackElement::Fire, AttackElement::Ice, AttackElement::Thunder };
		for( auto Elem : Elems )
		{
			OffsetRect( &StatusRect, 0, LineH );
			BMP( Elem ).BitBlt( hdc, StatusRect.left, StatusRect.top + ( (LineH - IconSize)/2 ) );
			SetTextColor( hdc, GUI::Color::White );
			SetBkMode( hdc, TRANSPARENT );
			DrawText( hdc, ( std::to_wstring( pChar->DmgReducePercentage( Elem ) ) + L"%" ).c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
		}
	}

	{//��Ԉُ�ϐ�
		OffsetRect( &StatusRect, 0, LineH + 12 );
		DrawText( hdc, L"��Ԉُ�ϐ� :", -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );

		constexpr int IconSize = 16;

		OffsetRect( &StatusRect, 0, LineH );
		BMP( FirstSpell::Poison ).BitBlt( hdc, StatusRect.left, StatusRect.top + ( (LineH - IconSize)/2 ) );
		SetTextColor( hdc, GUI::Color::White );
		SetBkMode( hdc, TRANSPARENT );
		DrawText( hdc, ( std::to_wstring( pChar->PoisonResistPercentage() ) + L"%" ).c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
	}
#if false
	{//�X�L��
		const auto *pSkill = TgtChar.PassiveSkill();
		if( pSkill )
		{
			OffsetRect( &StatusRect, 0, LineH + 18 );
			StatusRect.left = TopLeft[0];
			StatusRect.right = GC_W;

			auto spIcon = pSkill->Icon();
			spIcon->BitBlt( hdc, StatusRect.left, StatusRect.top );

			SetTextColor( hdc, RGB(255,255,255) );
			SetBkMode( hdc, TRANSPARENT );
			OffsetRect( &StatusRect, spIcon->GetW() + 16, 0 );
			DrawText( hdc, ( L"[" + pSkill->NameU16() + L"]") .c_str(), -1, &StatusRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );
			OffsetRect( &StatusRect, 0, LineH );
			DrawText( hdc, pSkill->DescTextU16().c_str(), -1, &StatusRect, DT_NOCLIP /*|DT_SINGLELINE*/|DT_VCENTER|DT_LEFT );
		}
	}
#endif
}

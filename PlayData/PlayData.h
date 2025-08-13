#pragma once

#include <array>
#include <string>
#include <memory>
#include "Misc/DataVec.h"
#include "ActProc/ActResult.h"
#include "GameContent/PartyChar.h"

/// <summary>
/// パーティの最大人数
/// </summary>
constexpr int PARTY_CAPACITY = 3;

/// <summary>
/// ストックのアイテム個数がこの値以上である場合に，「多すぎ」とする数．
///		<remarks>
///		「ストックの最大サイズ」ではない．
///		度を越えた個数のアイテムを所持しようとすることを抑制するための値である．
///		例えば店でのアイテム購入を「荷物が多すぎる」として制限するような用途の値．
///		</reamrks>
/// </summary>
constexpr int N_TOO_MANY_STOCK_ITEMS = 32;


/// <summary>
/// プレイ状況データ．
/// （セーブやロードの対象になるようなデータ）
/// </summary>
class PlayData
{
public:
	PlayData();
	~PlayData();

public:	//--- セーブとロード ----------------------------------------

	/// <summary>セーブ</summary>
	/// <param name="FilePathU8">セーブ先ファイル名</param>
	/// <returns>成否</returns>
	bool Save( const std::string &FilePathU8 ) const;

	/// <summary>ロード</summary>
	/// <param name="FilePathU8">ロードするセーブファイル名</param>
	/// <returns>ロード結果．ただし失敗時にはnullptrを返す</returns> 
	static std::unique_ptr<PlayData> Load( const std::string &FilePathU8 );


public:	//--- パーティ編成関連 --------------------------------------

	/// <summary>現座のパーティ</summary>
	/// <returns>現在のパーティ構成．0人という状態もあり得る</returns>
	const DataVec< GameContent::PartyCharID > &CurrParty() const {	return m_CurrParty;	}

	/// <summary>パーティ編成を変更</summary>
	/// <param name="Chars">
	/// パーティの人数分のキャラクタIDをパーティ内並び順で指定する（空vectorの指定も有効），
	/// * パーティ最大人数を超える要素数のデータを与えた場合，多すぎる分は無視される（：先頭側のみが使用される）．
	/// * 引数データに重複がある場合，動作は不定．（不正な入力であるが，メソッド内でチェックされない）
	/// </param>
	void SetupParty( const std::vector<GameContent::PartyCharID> &Chars );

public:	//--- キャラクタ, アイテムストックデータの現状を参照 --------------------------
	
	GameContent::PartyChar &Char( GameContent::PartyCharID ID ){	return m_Chars[(int)ID];	}
	const GameContent::PartyChar &Char( GameContent::PartyCharID ID ) const {	return m_Chars[(int)ID];	}

	DataVec< GameContent::ItemID > &ItemStock(){	return m_ItemStock;	}
	const DataVec< GameContent::ItemID > &ItemStock() const {	return m_ItemStock;	}

public:	//--- 所持金 ------------------------------------------------

	/// <summary>現在の所持金</summary>
	/// <returns>所持金．負の値もあり得る</returns>
	int Money() const {	return m_Money;	}

	/// <summary>所持金の増減．結果として所持金が負の値になり得る．</summary>
	/// <param name="delta">増減値</param>
	void AddMoney( int delta ){	m_Money += delta;	}

public:	//--- その他 ------------------------------------------------

	/// <summary>
	/// 宿屋に泊まった際の処理(各種回復等)．
	/// ただし，所持金の増減処理はここでは行わない → 別途AddMoney()で実施のこと．
	/// </summary>
	/// <returns>発生した回復効果</returns>
	std::vector< ActResult > ProcOfINN();

	/// <summary>
	/// 迷宮内のドアを通過できるか否か
	/// </summary>
	/// <param name="DoorLockLV">
	/// ドアの鍵LV(0～)．
	/// 0は鍵のかかっていないドア．
	/// 1以上は鍵のLVを示す．
	/// </param>
	/// <returns>通過できるか否か</returns>
	bool CanMoveThrough( int DoorLockLV ) const;

	//===============================================================
private:
	

private:
	//キャラクタ全員分のデータ．要素の順序は PartyCharID の要素の並び順と対応
	std::array< GameContent::PartyChar, GameContent::N_PARTY_CHAR_DEF > m_Chars;

	//現在のパーティ
	DataVec< GameContent::PartyCharID > m_CurrParty;
	//所持金
	int m_Money = 0;
	//アイテム Stock
	DataVec< GameContent::ItemID > m_ItemStock;


};

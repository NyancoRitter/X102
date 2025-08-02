#pragma once
#include <string>

namespace GameContent
{
	/// <summary>
	/// パーティキャラクタ固有ID．
	/// 値は 0 からの連番とする．
	/// </summary>
	enum class PartyCharID
	{
		AKINS,
		FANA,
		VISMATO,
		LIBER,
		IRY,
		ASLAY
	};

	/// <summary>定義されているパーティキャラクタの人数</summary>
	constexpr int N_PARTY_CHAR_DEF = 6;

	/// <summary>キャラクタの名前</summary>
	/// <param name="ID">キャラクタID</param>
	/// <returns>名前</returns>
	inline const std::wstring &NameStr( PartyCharID ID )
	{
		static const std::wstring Names[N_PARTY_CHAR_DEF] = {
			L"エイキンス",
			L"ファナ",
			L"ヴィスマト",
			L"リーベル",
			L"イリィ",
			L"アスレイ"
		};

		return Names[ (int)ID ];
	}
}

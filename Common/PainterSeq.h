#pragma once

#include "GUI/IPainter.h"
#include <vector>
#include <algorithm>

/// <summary>
/// GUI::IPainter* の列を保持し，Paint() メソッドで全ての要素の Paint() を呼ぶ
/// ……ということを行うだけのもの．
/// 
/// PushBack()された GUI::IPainter * が指す対象の寿命には全く関与しない．
/// </summary>
class PainterSeq
{
public:
	/// <summary>登録（列の末尾に追加）</summary>
	/// <param name="p">対象値．ただしnullptrを指定した場合には何もしない</param>
	void PushBack( const GUI::IPainter *p ){	if(p)m_Painters.push_back(p);	}
		
	/// <summary>
	/// 登録解除（列から削除）．
	/// 対象が複数存在する場合，最も先頭側（先にPushBackされた側）の１つが削除される．
	/// </summary>
	/// <param name="p">対象値</param>
	/// <returns>成否．引数値が列内に存在しなかった場合にはfalseを返す</returns>
	bool Remove( const GUI::IPainter *p )
	{
		auto iRemove = std::find( m_Painters.begin(), m_Painters.end(), p );
		if( iRemove == m_Painters.end() )return false;
		m_Painters.erase( iRemove );
		return true;
	}

	//
	void Clear(){	m_Painters.clear();	}
	bool Empty() const {	return m_Painters.empty();	}

	/// <summary>列に登録されている全 IPainter * の Paint() を呼ぶ</summary>
	/// <param name="hdc"></param>
	void Paint( HDC hdc ) const
	{
		for( const auto *p : m_Painters )
		{	p->Paint( hdc );	}
	}
private:
	std::vector< const GUI::IPainter* > m_Painters;
};


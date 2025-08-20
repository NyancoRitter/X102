#pragma once

#include "GUI/IPainter.h"
#include <vector>
#include <algorithm>

/// <summary>
/// GUI::IPainter* �̗��ێ����CPaint() ���\�b�h�őS�Ă̗v�f�� Paint() ���Ă�
/// �c�c�Ƃ������Ƃ��s�������̂��́D
/// 
/// PushBack()���ꂽ GUI::IPainter * ���w���Ώۂ̎����ɂ͑S���֗^���Ȃ��D
/// </summary>
class PainterSeq
{
public:
	/// <summary>�o�^�i��̖����ɒǉ��j</summary>
	/// <param name="p">�Ώےl�D������nullptr���w�肵���ꍇ�ɂ͉������Ȃ�</param>
	void PushBack( const GUI::IPainter *p ){	if(p)m_Painters.push_back(p);	}
		
	/// <summary>
	/// �o�^�����i�񂩂�폜�j�D
	/// �Ώۂ��������݂���ꍇ�C�ł��擪���i���PushBack���ꂽ���j�̂P���폜�����D
	/// </summary>
	/// <param name="p">�Ώےl</param>
	/// <returns>���ہD�����l������ɑ��݂��Ȃ������ꍇ�ɂ�false��Ԃ�</returns>
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

	/// <summary>��ɓo�^����Ă���S IPainter * �� Paint() ���Ă�</summary>
	/// <param name="hdc"></param>
	void Paint( HDC hdc ) const
	{
		for( const auto *p : m_Painters )
		{	p->Paint( hdc );	}
	}
private:
	std::vector< const GUI::IPainter* > m_Painters;
};


#pragma once

#include <memory>
#include <list>
#include "Effect.h"

class EffectList
{
public:
	bool empty() const {	return m_Effects.empty();	}
	void clear(){	m_Effects.clear();	}

	void PushFront( std::unique_ptr<IEffect> upEffect ){	m_Effects.emplace_front( std::move(upEffect) );	}
	void PushBack( std::unique_ptr<IEffect> upEffect ){	m_Effects.emplace_back( std::move(upEffect) );	}

	void Update()
	{
		auto i=m_Effects.begin();
		while( i!=m_Effects.end() )
		{
			bool Finished = !(*i)->Update();
			bool SuppressSubsequents = (*i)->SuppressSubsequents();

			if( Finished ){	i = m_Effects.erase(i);	}
			else{	i++;	}

			if( SuppressSubsequents )
			{	break;	}
		}
	}

	void Paint( HDC hdc ) const
	{
		for( const auto &up : m_Effects )
		{
			up->Paint( hdc );
			if( up->SuppressSubsequents() )break;
		}
	}

private:
	std::list< std::unique_ptr<IEffect> > m_Effects;
};

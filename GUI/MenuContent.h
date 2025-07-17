#pragma once

#include "IMenuContent.h"
#include "MenuItem.h"
#include <vector>

namespace GUI::Menu
{
	template< bool IsVertical, class ...ItemDrawParts >
	class MenuContent : public IMenuContent
	{
	public:
		MenuContent( const Vec2i &ItemSize ) : m_ItemSize(ItemSize) {}

	public:
		MenuContent &Clear(){	m_Items.clear();	return *this;	}
		MenuContent &Reserve( size_t n ){	m_Items.reserve(n);	return *this;	}

		MenuContent &Add( const ItemDrawParts &... ItemParts )
		{
			m_Items.emplace_back( ItemParts... );
			return *this;
		}

		MenuItem<ItemDrawParts...> &Item( int index )
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	public:	// IMenuContent Impl
		virtual bool IsVerticalMenu() const override {	return IsVertical;	}
		virtual Vec2i ItemSize() const override {	return m_ItemSize;	}
		virtual int nItems() const override {	return (int)m_Items.size();	}
		virtual int CursorPos() const override {	return m_CursorPos;	}
		virtual void CursorPos( int pos ) override {	m_CursorPos = ( pos<nItems()  ?  pos  :  -1 );	}
		virtual const IMenuItem &Item( int index ) const override
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	private:
		Vec2i m_ItemSize;
		int m_CursorPos = -1;
		std::vector< MenuItem<ItemDrawParts...> > m_Items;
	};

	//---

	template<class ...ItemDrawParts >
	using VMenuContent = MenuContent<true, ItemDrawParts... >;

	template<class ...ItemDrawParts >
	using HMenuContent = MenuContent<false, ItemDrawParts... >;
}
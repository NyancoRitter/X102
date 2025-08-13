#pragma once
#include <vector>

//DATA_ITEM_TYPE型のvectorを，２次元ぽく扱えるようにするだけのテンプレート
template < class DATA_ITEM_TYPE >
class UC2DIndexVector
{
	//構築と破壊
public:
	UC2DIndexVector() : m_SizeX(0), m_SizeY(0) {}
	UC2DIndexVector( unsigned int nx, unsigned int ny, const DATA_ITEM_TYPE &val=DATA_ITEM_TYPE() ){	assign(nx,ny,val);	}

public:
	//x*yにサイズ確保．
	//結果として領域拡大される場合には，増えた個所の値をvalにする．
	void resize( unsigned int nx, unsigned int ny, DATA_ITEM_TYPE val=DATA_ITEM_TYPE() )
	{
		if( nx!=m_SizeX || ny!=m_SizeY )
		{
			m_Data.resize( nx*ny, val );
			m_SizeX = nx;
			m_SizeY = ny;
		}
	}

	//サイズをx*y個にし，値を全てvalにする．
	void assign( unsigned int nx, unsigned int ny, const DATA_ITEM_TYPE &val )
	{
		m_Data.assign( nx*ny, val );
		m_SizeX = nx;
		m_SizeY = ny;
	}

	//サイズ参照
	unsigned int size_x() const {	return m_SizeX;	}
	unsigned int size_y() const {	return m_SizeY;	}
	bool empty() const {	return m_Data.empty();	}

	//クリア
	void clear()
	{
		m_Data.clear();
		m_SizeX = m_SizeY = 0;
	}

	//reserve
	void reserve( unsigned int nx, unsigned int ny )
	{	m_Data.reserve( nx*ny );	}

	//要素参照
#ifdef _DEBUG
	DATA_ITEM_TYPE &at( unsigned int x, unsigned int y ){	return m_Data.at( offset(x,y) );	}
	const DATA_ITEM_TYPE &at( unsigned int x, unsigned int y ) const {	return m_Data.at( offset(x,y) );	}
#else
	DATA_ITEM_TYPE &at( unsigned int x, unsigned int y ){	return m_Data[ offset(x,y) ];	}
	const DATA_ITEM_TYPE &at( unsigned int x, unsigned int y ) const {	return m_Data[ offset(x,y) ];	}
#endif

	//イテレータ
	typedef typename std::vector<DATA_ITEM_TYPE>::iterator	iterator;
	typedef typename std::vector<DATA_ITEM_TYPE>::const_iterator	const_iterator;
		//全データ範囲
	iterator begin(){	return m_Data.begin();	}
	iterator end(){	return m_Data.end();	}
	const_iterator begin() const {	return m_Data.begin();	}
	const_iterator end() const {	return m_Data.end();	}
		//あるy座標の行の範囲に関する
	iterator begin( unsigned int y ){	return m_Data.begin() + offset(0,y);	}
	iterator end( unsigned int y ){	return m_Data.begin() + offset(0,y+1);	}
	const_iterator begin( unsigned int y ) const {	return m_Data.begin() + offset(0,y);	}
	const_iterator end( unsigned int y ) const {	return m_Data.begin() + offset(0,y+1);	}

	//データ
private:
	std::vector< DATA_ITEM_TYPE > m_Data;
	unsigned int m_SizeX;
	unsigned int m_SizeY;

private:
	unsigned int offset( unsigned int x, unsigned int y ) const {	return ( x + y*m_SizeX );	}
};

//[EOF]
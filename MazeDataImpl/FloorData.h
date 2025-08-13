#pragma once

#include "MazeRendering/IMazeData.h"
#include "Parts/Vec2D.h"
#include "MazeDataDefs.h"
#include "Parts/UC2DIndexVector.h"
#include "Direction.h"
#include <map>
#include <optional>

namespace MazeDataImpl
{
	//迷路のデータ（１フロア分）具体実装
	class FloorData : public PSEUDO_3D_MAZE::IMazeData
	{
	public:
		FloorData( unsigned int size_x, unsigned int size_y )
			: m_EdgeData(size_x, size_y)
		{}

	public:	//PSEUDO_3D_MAZE::IMazeData
		virtual int NorthSideEdgeAttr( int x, int y ) const override {	return GetEdgeAttr( Vec2i(x,y), Direction::NORTH );	}
		virtual int SouthSideEdgeAttr( int x, int y ) const override {	return GetEdgeAttr( Vec2i(x,y), Direction::SOUTH );	}
		virtual int WestSideEdgeAttr( int x, int y ) const override {	return GetEdgeAttr( Vec2i(x,y), Direction::WEST );	}
		virtual int EastSideEdgeAttr( int x, int y ) const override {	return GetEdgeAttr( Vec2i(x,y), Direction::EAST );	}

	public:
		//迷路の広さ
		unsigned int SizeX() const {	return m_EdgeData.size_x();	}
		unsigned int SizeY() const {	return m_EdgeData.size_y();	}

		//指定マスの指定方向のエッジ属性の取得
		//（意味合いについては PSEUDO_3D_MAZE::IMazeData のメソッドに倣う）
		int GetEdgeAttr( const Vec2i &Pos, Direction EdgeDir ) const
		{
			if( Pos[0]<0 || Pos[1]<0 )return (int)EdgeAttr::WALL;
			unsigned int x = (unsigned int)Pos[0];
			unsigned int y = (unsigned int)Pos[1];
			if( x>=m_EdgeData.size_x()  ||  y>=m_EdgeData.size_y() )return (int)EdgeAttr::WALL;

			return (  ( m_EdgeData.at(x,y) >> 4*(int)EdgeDir ) & 0b1111  );
		}

		//指定位置に存在するオブジェクトの取得
		std::optional<ObjInfo> GetObjectAt( const Vec2i &Pos ) const
		{
			auto iObjType = m_Objects.find( Pos );
			if( iObjType == m_Objects.end() )return std::nullopt;

			return iObjType->second;
		}

		//このフロアのオブジェクト一覧
		const std::map< Vec2i, ObjInfo > &GetObjectInfo() const {	return m_Objects;	}

	public:
		//指定マスのエッジ情報を設定
		void SetEdgeAttr( unsigned int x, unsigned int y, const unsigned char (&Attrs)[4] )
		{
			if( x>=m_EdgeData.size_x()  ||  y>=m_EdgeData.size_y() )return;
			uint16_t Val = 0;
			for( int i=3; i>=0; --i )
			{
				Val <<= 4;
				Val |= uint16_t(Attrs[i] & 0b1111);
			}
			m_EdgeData.at(x,y) = Val;
		}

		//指定マスに存在するオブジェクト情報を設定
		void RegisterObject( const Vec2i &Pos, const ObjInfo &Obj ){	m_Objects[Pos] = Obj;	}
		void RemoveObject( const Vec2i &Pos ){	m_Objects.erase(Pos);	}

	private:
		//このフロアの全マス分のエッジ情報
		//uint16_t : １マスのエッジの情報を，各方向4bitの値として保持する．
		UC2DIndexVector< uint16_t > m_EdgeData;

		//オブジェクト群
		std::map< Vec2i, ObjInfo > m_Objects;
	};
}
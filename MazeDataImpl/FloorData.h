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
	//���H�̃f�[�^�i�P�t���A���j��̎���
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
		//���H�̍L��
		unsigned int SizeX() const {	return m_EdgeData.size_x();	}
		unsigned int SizeY() const {	return m_EdgeData.size_y();	}

		//�w��}�X�̎w������̃G�b�W�����̎擾
		//�i�Ӗ������ɂ��Ă� PSEUDO_3D_MAZE::IMazeData �̃��\�b�h�ɕ키�j
		int GetEdgeAttr( const Vec2i &Pos, Direction EdgeDir ) const
		{
			if( Pos[0]<0 || Pos[1]<0 )return (int)EdgeAttr::WALL;
			unsigned int x = (unsigned int)Pos[0];
			unsigned int y = (unsigned int)Pos[1];
			if( x>=m_EdgeData.size_x()  ||  y>=m_EdgeData.size_y() )return (int)EdgeAttr::WALL;

			return (  ( m_EdgeData.at(x,y) >> 4*(int)EdgeDir ) & 0b1111  );
		}

		//�w��ʒu�ɑ��݂���I�u�W�F�N�g�̎擾
		std::optional<ObjInfo> GetObjectAt( const Vec2i &Pos ) const
		{
			auto iObjType = m_Objects.find( Pos );
			if( iObjType == m_Objects.end() )return std::nullopt;

			return iObjType->second;
		}

		//���̃t���A�̃I�u�W�F�N�g�ꗗ
		const std::map< Vec2i, ObjInfo > &GetObjectInfo() const {	return m_Objects;	}

	public:
		//�w��}�X�̃G�b�W����ݒ�
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

		//�w��}�X�ɑ��݂���I�u�W�F�N�g����ݒ�
		void RegisterObject( const Vec2i &Pos, const ObjInfo &Obj ){	m_Objects[Pos] = Obj;	}
		void RemoveObject( const Vec2i &Pos ){	m_Objects.erase(Pos);	}

	private:
		//���̃t���A�̑S�}�X���̃G�b�W���
		//uint16_t : �P�}�X�̃G�b�W�̏����C�e����4bit�̒l�Ƃ��ĕێ�����D
		UC2DIndexVector< uint16_t > m_EdgeData;

		//�I�u�W�F�N�g�Q
		std::map< Vec2i, ObjInfo > m_Objects;
	};
}
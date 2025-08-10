#pragma once

#include <vector>

/// <summary>
/// std::vector<T_Data> �����p�r�ɍ��킹�Ă����g���Ȃ���������
/// 
/// * �f�[�^�ő���ɐ���
/// * ����ɐ���
/// </summary>
/// <typeparam name="T_Data">�ێ�����f�[�^�̌^</typeparam>
template< class T_Data >
class DataVec
{
public:
	/// <summary>ctor</summary>
	/// <param name="Max">�ێ��\�f�[�^�ő��</param>
	DataVec( size_t Max ) : m_MaxSize(Max) {}

public:
	//�ۗL�\�f�[�^�ő��
	size_t MaxSize() const {	return m_MaxSize;	}

public: //�ۗL vector �ւ̒P���ȏ����ڏ�
	size_t size() const {	return m_Vec.size();	}
	bool empty() const {	return m_Vec.empty();	}
	void clear(){	m_Vec.clear();	}

	const T_Data &operator[]( size_t index ) const {	return m_Vec[index];	}
	T_Data &operator[]( size_t index ){	return m_Vec[index];	}

	auto begin()->auto{	return m_Vec.begin();	}
	auto begin() const ->auto{	return m_Vec.begin();	}
	auto end()->auto{	return m_Vec.end();	}
	auto end() const ->auto{	return m_Vec.end();	}

	//�ێ��f�[�^�̃R�s�[��Ԃ�
	std::vector<T_Data> DataClone() const {	return m_Vec;	}

public:	//���ڑ���

	/// <summary>
	/// �����ɒǉ��D
	/// ���������Ƀf�[�^�ő���ɒB���Ă���ꍇ�ɂ͎��s����D
	/// </summary>
	/// <param name="d">�ǉ�����f�[�^</param>
	/// <returns>���ہD�ǉ��ł����ꍇ��true</returns>
	bool PushBack( const T_Data &d )
	{
		if( full() )return false;
		m_Vec.push_back(d);
		return true;
	}

	/// <summary>
	/// �ێ��f�[�^���ۂ��ƍX�V����D
	/// �����������̃T�C�Y�� MaxSize() �𒴂���ꍇ�ɂ͉���������false��Ԃ��D
	/// </summary>
	/// <param name="Data">�ێ�������f�[�^</param>
	/// <returns>���ہDfalse��Ԃ����ꍇ�C�����f�[�^�͍X�V����Ȃ�</returns>
	bool Assign( std::vector<T_Data> Data )
	{
		if( Data.size() > MaxSize() )return false;
		m_Vec = Data;
		return true;
	}

	/// <summary>���ڍ폜</summary>
	/// <param name="index">�Ώ�index</param>
	void RemoveAt( int index ){	m_Vec.erase( std::next( m_Vec.begin(), index ) );	}

	/// <summary>�w�荀�ڂ𖖔��Ɉړ�����i�����p�̃��\�b�h�j</summary>
	/// <param name="index">�Ώ�index</param>
	/// <returns>
	/// ���ہD
	/// �f�[�^����̏ꍇ��C�w��index�������v�f���w���ꍇ�ɂ͉���������false��Ԃ��D
	/// </returns>
	bool MoveItemToBack( int index )
	{
		if( empty() || index==(int)size()-1 )return false;

		T_Data Tgt = m_Vec[index];
		RemoveAt( index );
		m_Vec.push_back( Tgt );
		return true;
	}

public:	//���̑��w���p�I��

	/// <summary>�ő�f�[�^���ɒB���Ă��邩�ۂ�</summary>
	/// <returns></returns>
	bool full() const {	return ( size()==MaxSize() );	}

	/// <summary>�w��̃f�[�^���܂܂�Ă��邩�ۂ�</summary>
	/// <param name="d">�Ώۃf�[�^</param>
	/// <returns>d�Ɓu�����v���̂��P�ȏ�ێ�����Ă��邩�ۂ�</returns>
	bool Contains( const T_Data &d ) const
	{
		return ( std::find( m_Vec.begin(), m_Vec.end(), d ) != m_Vec.end() );
	}

private:
	size_t m_MaxSize;
	std::vector< T_Data > m_Vec;
};

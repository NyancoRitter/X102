#pragma once

#include <vector>

/// <summary>
/// std::vector<T_Data> を特定用途に合わせてしか使えなくしたもの
/// 
/// * データ最大個数に制限
/// * 操作に制限
/// </summary>
/// <typeparam name="T_Data">保持するデータの型</typeparam>
template< class T_Data >
class DataVec
{
public:
	/// <summary>ctor</summary>
	/// <param name="Max">保持可能データ最大個数</param>
	DataVec( size_t Max ) : m_MaxSize(Max) {}

public:
	//保有可能データ最大個数
	size_t MaxSize() const {	return m_MaxSize;	}

public: //保有 vector への単純な処理移譲
	size_t size() const {	return m_Vec.size();	}
	bool empty() const {	return m_Vec.empty();	}
	void clear(){	m_Vec.clear();	}

	const T_Data &operator[]( size_t index ) const {	return m_Vec[index];	}
	T_Data &operator[]( size_t index ){	return m_Vec[index];	}

	auto begin()->auto{	return m_Vec.begin();	}
	auto begin() const ->auto{	return m_Vec.begin();	}
	auto end()->auto{	return m_Vec.end();	}
	auto end() const ->auto{	return m_Vec.end();	}

	//保持データのコピーを返す
	std::vector<T_Data> DataClone() const {	return m_Vec;	}

public:	//項目操作

	/// <summary>
	/// 末尾に追加．
	/// ただし既にデータ最大個数に達している場合には失敗する．
	/// </summary>
	/// <param name="d">追加するデータ</param>
	/// <returns>成否．追加できた場合はtrue</returns>
	bool PushBack( const T_Data &d )
	{
		if( full() )return false;
		m_Vec.push_back(d);
		return true;
	}

	/// <summary>
	/// 保持データを丸ごと更新する．
	/// ただし引数のサイズが MaxSize() を超える場合には何もせずにfalseを返す．
	/// </summary>
	/// <param name="Data">保持させるデータ</param>
	/// <returns>成否．falseを返した場合，内部データは更新されない</returns>
	bool Assign( std::vector<T_Data> Data )
	{
		if( Data.size() > MaxSize() )return false;
		m_Vec = Data;
		return true;
	}

	/// <summary>項目削除</summary>
	/// <param name="index">対象index</param>
	void RemoveAt( int index ){	m_Vec.erase( std::next( m_Vec.begin(), index ) );	}

	/// <summary>指定項目を末尾に移動する（整理用のメソッド）</summary>
	/// <param name="index">対象index</param>
	/// <returns>
	/// 成否．
	/// データが空の場合や，指定indexが末尾要素を指す場合には何もせずにfalseを返す．
	/// </returns>
	bool MoveItemToBack( int index )
	{
		if( empty() || index==(int)size()-1 )return false;

		T_Data Tgt = m_Vec[index];
		RemoveAt( index );
		m_Vec.push_back( Tgt );
		return true;
	}

public:	//その他ヘルパ的な

	/// <summary>最大データ個数に達しているか否か</summary>
	/// <returns></returns>
	bool full() const {	return ( size()==MaxSize() );	}

	/// <summary>指定のデータが含まれているか否か</summary>
	/// <param name="d">対象データ</param>
	/// <returns>dと「同じ」ものが１つ以上保持されているか否か</returns>
	bool Contains( const T_Data &d ) const
	{
		return ( std::find( m_Vec.begin(), m_Vec.end(), d ) != m_Vec.end() );
	}

private:
	size_t m_MaxSize;
	std::vector< T_Data > m_Vec;
};

#pragma once

#include <string>
#include <memory>
#include <vector>
class CMonoBMP;

namespace ResManage
{
	/// <summary>
	/// モノクロBMPをbitmapファイルからロードする．
	/// ただしファイルロードが失敗した場合でも何らかのダミー画像を返す．
	/// </summary>
	/// <param name="FilePathNameU16">ファイルパス</param>
	/// <returns></returns>
	std::unique_ptr<CMonoBMP> LoadMonoBMP_or_Dummy( const std::wstring &FilePathNameU16 );

	/// <summary>
	/// モノクロBMP群の保持者．
	/// 各画像に関して要求された初回にロードする．
	/// </summary>
	class MonoBmpHolder
	{
	public:
		/// <summary>生成ヘルパ</summary>
		/// <param name="ImgDirU16">画像ファイル群のあるパス（UTF16, 末尾がパス区切り記号で終わっていること）</param>
		/// <param name="ImgFileNamesU16">画像ファイル群のファイル名(UTF16)</param>
		/// <returns></returns>
		static MonoBmpHolder Create(
			const std::wstring &ImgDirU16,
			std::initializer_list<std::wstring> ImgFileNamesU16
		)
		;//{	return MonoBmpHolder( ImgDirU16, ImgFileNamesU16.begin(), ImgFileNamesU16.end() );	}

	public:
		/// <summary>
		/// ctor.最初に画像ファイル名を全て指定しておく必要がある．
		/// </summary>
		/// <typeparam name="FileNameU16Iter">ファイル名指定用イテレータの型</typeparam>
		/// <param name="ImgDirU16">画像ファイル群のあるパス（UTF16, 末尾がパス区切り記号で終わっていること）</param>
		/// <param name="begin">[begin,end) で画像ファイル群のファイル名(UTF16)を指定</param>
		/// <param name="end">[begin,end) で画像ファイル群のファイル名(UTF16)を指定</param>
		template<class FileNameU16Iter >
		MonoBmpHolder(
			const std::wstring &ImgDirU16,
			FileNameU16Iter begin, FileNameU16Iter end
 )
			: m_ImgDirU16(ImgDirU16)
			, m_ImgFileNamesU16( begin, end )
		{	m_Bank.resize( m_ImgFileNamesU16.size() );	}

		~MonoBmpHolder();

	public:
		size_t size() const {	return m_Bank.size();	}

		/// <summary>
		/// 画像の取得．引数index値に対して初回の呼び出しであればここで画像読込が実施される．
		/// </summary>
		/// <param name="index">画像index．ctorに指定したファイル名の並びと対応．</param>
		/// <returns>
		/// 画像読み込みに失敗した場合には，何かしらの代替のダミー画像が返される．
		/// </returns>
		const CMonoBMP &operator[]( int index );

	private:
		MonoBmpHolder( const MonoBmpHolder& ) = delete;
		MonoBmpHolder &operator=( const MonoBmpHolder& ) = delete;
	private:
		std::vector< std::unique_ptr<const CMonoBMP> > m_Bank;
		std::vector< std::wstring > m_ImgFileNamesU16;
		std::wstring m_ImgDirU16;
	};
}

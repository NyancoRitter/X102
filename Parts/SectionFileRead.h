#pragma once
#include <map>
#include <fstream>
#include <string>
#include <filesystem>

//INIっぽいセクション区切りファイルの読み込み．
//
// * 各行の先頭および末尾のスペースとタブは除去される．
//  （以下の記述は除去後の内容に関する）
// * セクションは
//  [SectionName]
//  という形でセクション名を[]で囲んだ記述のみの行で開始．
//		* ファイル内で最初のセクション開始行よりも前に書かれた記述は，
//		  セクション名が空文字列なセクション内の記述と見なされる．
// * 空行は無視される．
// * コメント行も無視される
//
namespace SectionFile
{

	//文字列の前後の不要なスペース等を除去する作業関数
	inline
	std::string EraseSpaceAtHeadAndTail( const std::string &rSrc, const char EraseChars[] = " \t\n\r" )
	{
		const auto spos = rSrc.find_first_not_of( EraseChars );
		if( spos==std::string::npos ){	return "";	}
		const auto epos = rSrc.find_last_not_of( EraseChars );
		return rSrc.substr( spos, epos-spos+1 );
	}


	//ファイルローダ
	//	SectionData_T :
	//		各セクションの読込結果データの型．
	//		デフォルトコンストラクトや empty() による空判定が必要．
	template< class SectionData_T >
	class FileLoader
	{
	public:
		//ファイル内容格納用データ構造
		//map< SectionName, セクション内の行のデータ >
		using FileData = std::map< std::string, SectionData_T >;

	public:
		FileLoader() : m_LastLineNum(0) {}
		virtual ~FileLoader() = default;
		FileLoader( const FileLoader & ) = delete;
		FileLoader &operator=( const FileLoader & ) = delete;

	public:
		//Load()で最後に読んだ行の情報．エラー時に参照する用．
		//GetLastLineNum()が負の値を返す場合は，ファイルオープンエラー．
		int GetLastLineNum() const {	return m_LastLineNum;	}
		std::string GetLastLineStr() const {	return m_LastLineStr;	}

	protected:
		//Load() 内から呼ばれるコメント行の判定用処理．
		//trueを返した場合，その行は単に無視される．
		//引数は，先頭と末尾の空白文字を除去済みの空でない行文字列．
		virtual bool IsCommentLine( const std::string &LineStr ) const
		{	//デフォルト実装では "//" から始まる行をコメント行とする
			//if( LineStr[0]==';' || LineStr[0]=='#' )return  true;
			if( LineStr.size()>=2  &&  LineStr[0]=='/'  &&  LineStr[1]=='/' )return true;
			return false;
		}

		//Load() 内から呼ばれる行のパース処理．
		//引数は，先頭と末尾の空白文字を除去済みの空でない行文字列．
		//成功時は処理結果を SecData に 反映/格納 して true を返す．
		//false を返した場合，Load() はエラーとして終了する．
		virtual bool ParseLine( const std::string &LineStr, SectionData_T &SecData, int LineNo ) = 0;

	public:
		/// <summary>
		/// ファイル読込
		/// * 成功時には rDstData に読込結果を格納して true を返す．
		/// * false を返す場合， rDstData の内容は保証されない．
		/// </summary>
		/// <param name="rFilePathNameU8">ファイルパス (UTF8)</param>
		/// <param name="rDstData">結果受け取り</param>
		/// <returns>成否</returns>
		bool Load( const std::string &rFilePathNameU8, FileData &rDstData )
		{
			m_LastLineStr.clear();
			std::ifstream File( std::filesystem::u8path(rFilePathNameU8) );
			if( !File.is_open() ){	m_LastLineNum = -1;	return false;	}

			SectionData_T NoSecParams;	//セクション名の無い(いきなり書かれている)行の一時格納用
			SectionData_T *pCurrSection = &NoSecParams;
			m_LastLineNum = 0;
			rDstData.clear();

			std::string LineStr;
			std::string Key,Value;
			while( std::getline( File, m_LastLineStr ) )
			{
				++m_LastLineNum;

				//前後の不要なスペース等を除去
				LineStr = EraseSpaceAtHeadAndTail( m_LastLineStr );
				if( LineStr.empty() )continue;	//空行は無視
				if( IsCommentLine( LineStr ) )continue;	//コメント行判定

				//セクション判定
				if( LineStr[0]=='[' && (*LineStr.rbegin())==']' )
				{
					std::string SectionName = EraseSpaceAtHeadAndTail( LineStr.substr( 1, LineStr.length()-2 ) );
					if( SectionName.empty() )return false;
					pCurrSection = &rDstData[ SectionName ];
					continue;
				}

				//行内容をパース
				if( !ParseLine( LineStr, *pCurrSection, m_LastLineNum ) )
				{	return false;	}
			}
			File.close();

			//セクション名の無い行のデータがあれば結果に追加
			if( !NoSecParams.empty() )
			{
				rDstData.insert( std::pair< std::string, SectionData_T >( "", std::move(NoSecParams) ) );
			}

			return File.eof();
		}

	private:
		std::string m_LastLineStr;
		int m_LastLineNum;
	};

}
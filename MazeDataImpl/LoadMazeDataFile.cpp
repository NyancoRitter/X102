#include "framework.h"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <list>
#include <vector>
#include <optional>

#include "LoadMazeDataFile.h"
#include "Parts/SectionFileRead.h"
#include "Parts/CvtStr2T.h"

#include "Parts/IniLikeFile.h"	//※ここにある関数 SplitVal() を使うため．定義されているファイルローダは使わない

namespace
{
	using SectionData = std::vector< std::string >;

	class MapFileLoader : public SectionFile::FileLoader<SectionData>
	{
		virtual bool ParseLine( const std::string &LineStr, SectionData &SecData, int LineNo ) override
		{
			SecData.emplace_back( LineStr );
			return true;
		}
	};

	//SectionData の内容を，
	//全文字列が INI っぽい "Key = Value" という記述であるものとして，
	//'=' を境に分割する処理．
	//戻り値は｛ Key が '=' の左側，Value が '=' の右側｝というmapになる．
	//
	//INIっぽい記述ではない文字列が存在した場合，その文字列そのものをエラー文字列とする std::runtime_error を送出する．
	std::map<std::string,std::string> InterpretSectionAsIni( const SectionData &SecData ) 
	{
		std::map<std::string,std::string> Result;
		for( const auto &LineStr : SecData )
		{
			const auto length = LineStr.length();

			//'='の場所を見つける
			const auto EqPos = LineStr.find_first_of( '=' );
			if( EqPos == std::string::npos  ||  EqPos==0  ||  EqPos==length-1 )
			{	throw std::runtime_error( LineStr );	}

			//'='の左側
			std::string Key = SectionFile::EraseSpaceAtHeadAndTail( LineStr.substr( 0, EqPos ) );
			if( Key.empty() )
			{	throw std::runtime_error( LineStr );	}

			if( Key.find_first_of( '\"' ) != std::string::npos )	//Keyにはダブルクォートを含めてははダメということにする
			{	throw std::runtime_error( LineStr );	}
			
			//'='の右側
			std::string Value = SectionFile::EraseSpaceAtHeadAndTail( LineStr.substr( EqPos+1, length-EqPos-1 ) );
			if( Value.empty() )
			{	throw std::runtime_error( LineStr );	}

			//
			Result[ Key ] = Value;
		}
		return Result;
	}

	using namespace MazeDataImpl;

	inline unsigned char DoorOrNone( char c )
	{
		if( '0'<=c && c<='9' )
		{	return (unsigned char)EdgeAttr::DOOR + (c-'0');	}
		else
		{	return (unsigned char)EdgeAttr::NONE;	}
	}

	inline unsigned char EastAttr( char c )
	{
		if( c=='#' || c=='>' )return (unsigned char)EdgeAttr::WALL;
		return DoorOrNone( c );
	}

	inline unsigned char WestAttr( char c )
	{
		if( c=='#' || c=='<' )return (unsigned char)EdgeAttr::WALL;
		return DoorOrNone( c );
	}

	inline unsigned char NorthAttr( char c )
	{
		if( c=='#' || c=='^' )return (unsigned char)EdgeAttr::WALL;
		return DoorOrNone( c );
	}

	inline unsigned char SouthAttr( char c )
	{
		if( c=='#' || c=='v' )return (unsigned char)EdgeAttr::WALL;
		return DoorOrNone( c );
	}

	inline std::optional<ObjType> Obj( char c )
	{
		switch( c )
		{
		case 'U':	return ObjType::UP_STAIR;	break;
		case 'D':	return ObjType::DOWN_STAIR;	break;
		default:	break;
		}
		return std::nullopt;
	}
}

namespace MazeDataImpl
{
	std::vector< FloorData > LoadMazeMapFile( const std::string &MazeFilePathNameU8, Vec2i &StartPos, Direction &StartDir )
	{
		MapFileLoader::FileData Data;
		{
			MapFileLoader Loader;
			if( !Loader.Load( MazeFilePathNameU8, Data ) )
			{
				if( Loader.GetLastLineNum()<0 )
				{	throw std::runtime_error( u8"File Open Err : " +  MazeFilePathNameU8 );	}
				else
				{	throw std::runtime_error( u8"Err at Line " + std::to_string( Loader.GetLastLineNum() ) + u8":\r\n" + Loader.GetLastLineStr() );	}
			}
		}

		std::vector< FloorData > Ret;

		{//[MazeData]Section
			const std::string SectionName = u8"MazeData";
			auto iSection = Data.find( SectionName );
			if( iSection == Data.end() )
			{	throw std::runtime_error( u8"No `" + SectionName + u8"` Section" );	}

			std::map<std::string,std::string> IniLikeDescs;
			try
			{	IniLikeDescs = InterpretSectionAsIni( iSection->second );	}
			catch( std::runtime_error &ex )
			{	throw std::runtime_error( u8"`" + SectionName + u8"` Section SyntaxErr : [" + ex.what() + u8"]" );	}

			auto Vals = IniLikeFile::SplitVal( IniLikeDescs, u8"StartPos" );
			{
				int Dir;
				if( !Words2Vals( Vals.begin(), Vals.end(), StartPos[0], StartPos[1], Dir ) )
				{	throw std::runtime_error( u8"`StartPos` Parse Err" );	}

				StartDir = (Direction)( Dir % 4 );
			}

			Vals = IniLikeFile::SplitVal( IniLikeDescs, "Size" );
			int Size_x, Size_y, nFloor;
			if( !Words2Vals( Vals.begin(), Vals.end(), Size_x, Size_y, nFloor ) )
			{	throw std::runtime_error( u8"`Size` Parse Err" );	}

			Ret.assign( nFloor, FloorData( Size_x, Size_y ) );
		}

		//[MazeX]Section, where X is 0-based floor index
		for( int iFloor=0; iFloor<(int)Ret.size(); ++iFloor )
		{
			const std::string SectionName = u8"Maze" + std::to_string(iFloor);
			auto iSection = Data.find( SectionName );
			if( iSection == Data.end() )
			{	throw std::runtime_error( u8"No `" + SectionName + u8"` Section" );	}

			const SectionData &Lines = iSection->second;
			auto &ThisFloor = Ret[ iFloor ];
			const unsigned int SizeX = ThisFloor.SizeX();
			const unsigned int SizeY = ThisFloor.SizeY();
			{//セクション内の行数と各行の長さの妥当性チェック
				const size_t ValidLineLength = 2*SizeX + 1;
				const size_t nValidLines = 2*SizeY + 1;
				
				if( Lines.size() != nValidLines )
				{	throw std::runtime_error( u8"Num of lines in `" + SectionName + u8"` Section is invalid" );	}

				if( std::any_of( Lines.begin(), Lines.end(), [ValidLineLength](const std::string &line)->bool{	return line.size()!=ValidLineLength;	} ) )
				{	throw std::runtime_error( u8"Invalid line length in `" + SectionName + u8"` Section" );	}
			}

			for( unsigned int y=0; y<SizeY; ++y )
			{
				const unsigned int iy = 1 + 2*y;
				for( unsigned int x=0; x<SizeX; ++x )
				{
					unsigned char EdgeAttr[4] = { 0,0,0,0 };	//E,S,W,N

					const unsigned int ix = 1 + 2*x;
					EdgeAttr[ (int)Direction::EAST ] = EastAttr( Lines[iy][ix+1] );
					EdgeAttr[ (int)Direction::WEST ] = WestAttr( Lines[iy][ix-1] );
					EdgeAttr[ (int)Direction::NORTH ] = NorthAttr( Lines[iy-1][ix] );
					EdgeAttr[ (int)Direction::SOUTH ] = SouthAttr( Lines[iy+1][ix] );
					ThisFloor.SetEdgeAttr( x,y, EdgeAttr );

					auto obj = Obj( Lines[iy][ix] );
					if( obj )
					{	ThisFloor.RegisterObject( Vec2i{(int)x,(int)y}, { obj.value(), 255u } );	}
				}
			}
		}
		//
		return Ret;
	}

}

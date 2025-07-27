#pragma once
#include <string>

/// <summary>Exeのあるディレクトリのパス文字列．</summary>
/// <returns>UTF16．末尾にパス区切り記号 (/) が付いた文字列</returns>
const std::wstring &ExeDirU16();

/// <summary>Exeのあるディレクトリのパス文字列．</summary>
/// <returns>UTF8．末尾にパス区切り記号 (/) が付いた文字列</returns>
const std::string &ExeDirU8();

//---

/// <summary>画像データ配置ディレクトリのパス文字列．</summary>
/// <returns>UTF16．末尾にパス区切り記号 (/) が付いた文字列</returns>
std::wstring ImgDirU16();


//
///// <summary>
///// データファイル
/////   * マップデータファイル
/////   * イベント定義ファイル
/////   * イベントスクリプトファイル
///// を配置すべきディレクトリのパス
///// </summary>
///// <returns>末尾にパス区切り記号 (/) が付いた文字列<</returns>
//std::string DataFileDirU8();
//
///// <summary>マップデータファイルのパス</summary>
///// <returns>UTF8</returns>
//std::string MazeMapFilePathU8();
//
///// <summary>イベント定義ファイルのパス</summary>
///// <returns>UTF8</returns>
//std::string EventDefinitionFilePathU8();
//
///// <summary>イベントスクリプトファイルのパス</summary>
///// <returns>UTF8</returns>
//std::string EventScriptFilePathU8( int EventID );
//
///// <summary>ゲームストーリー説明用イベントスクリプトファイルのパス</summary>
///// <returns>UTF8</returns>
//inline std::string GameStroyDescEventScriptFilePathU8(){	return EventScriptFilePathU8(998);	}
//
///// <summary>ゲームクリア時用イベントスクリプトファイルのパス</summary>
///// <returns>UTF8</returns>
//inline std::string GameClearEventScriptFilePathU8(){	return EventScriptFilePathU8(999);	}
//
///// <summary>セーブファイル配置ディレクトリのパス</summary>
///// <returns>末尾にパス区切り記号 (/) が付いた文字列<</returns>
//std::string GameSaveFileDirU8();
//
///// <summary>セーブファイルのパス</summary>
///// <returns>UTF8</returns>
//std::string GameSaveFilePathU8();
//
///// <summary>サウンドファイル群配置ディレクトリのパス</summary>
///// <returns>末尾にパス区切り記号 (/) が付いた文字列<</returns>
//std::wstring SoundDirU16();
//
///// <summary>BGM,SEのファイル群の名称が書かれているファイルへのパス</summary>
///// <returns>UTF8</returns>
//std::string SoundEnumerationFileOathU8();


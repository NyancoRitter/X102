#pragma once
#include <array>
#include <string>
#include <fstream>
#include <mmsystem.h>

#pragma comment( lib, "Winmm.lib" )

//wavファイルを読込んでおいて PlaySound() で鳴らすやつ
template< size_t N >	//N : 扱う音の個数
class Sound
{
public:
	Sound()
	{
		for( size_t i=0; i<N; ++i )m_SoundBuffs[i]=nullptr;
	}

	~Sound()
	{
		Stop();
		for( size_t i=0; i<N; ++i ){	delete[] m_SoundBuffs[i];	}
	}

	//index番目の音データを読込み．
	//（既に読込んでいる場合はデータ上書きとなる）
	bool Load( size_t index, const std::wstring &WaveFilePath )
	{
		std::ifstream File( WaveFilePath.c_str(), std::ifstream::binary );
		if( !File.is_open() )return false;
		auto FileSize = File.seekg( 0, std::ios::end ).tellg();
		File.seekg( 0, std::ios::beg );

		char *Buff = new char[ FileSize ];
		File.read( Buff, FileSize );
		File.close();

		delete[] m_SoundBuffs[index];
		m_SoundBuffs[index] = Buff;
		return true;
	}

	//index番目の音データが読み込まれているか否か
	bool Loaded( size_t index ) const {	return m_SoundBuffs[index];	}

	//音の再生．読込めていない場合には何もしない
	void Play( size_t index ) const
	{
		const char *Buff = m_SoundBuffs[index];
		if( Buff )
		{	PlaySoundA( Buff, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT );	}
	}

	//再生中の音の停止
	void Stop() const {	PlaySound(NULL,NULL,0);	}

private:
	std::array< char*, N > m_SoundBuffs;	//※読込めていない音はnullptrとする
};

#pragma once
#include <array>
#include <string>
#include <fstream>
#include <mmsystem.h>

#pragma comment( lib, "Winmm.lib" )

//wav�t�@�C����Ǎ���ł����� PlaySound() �Ŗ炷���
template< size_t N >	//N : �������̌�
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

	//index�Ԗڂ̉��f�[�^��Ǎ��݁D
	//�i���ɓǍ���ł���ꍇ�̓f�[�^�㏑���ƂȂ�j
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

	//index�Ԗڂ̉��f�[�^���ǂݍ��܂�Ă��邩�ۂ�
	bool Loaded( size_t index ) const {	return m_SoundBuffs[index];	}

	//���̍Đ��D�Ǎ��߂Ă��Ȃ��ꍇ�ɂ͉������Ȃ�
	void Play( size_t index ) const
	{
		const char *Buff = m_SoundBuffs[index];
		if( Buff )
		{	PlaySoundA( Buff, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT );	}
	}

	//�Đ����̉��̒�~
	void Stop() const {	PlaySound(NULL,NULL,0);	}

private:
	std::array< char*, N > m_SoundBuffs;	//���Ǎ��߂Ă��Ȃ�����nullptr�Ƃ���
};

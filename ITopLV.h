#pragma once
#include <string>

class PlayData;

/// <summary>
/// ITopLV::ChangeBGM()�̈����Ɏg����萔
/// </summary>
enum class BGMSpecifier
{
	Title,
	Town,
	Maze,
	Battle1, Battle2, Battle3,
	GameClear
};

/// <summary>
/// �e�V�[���ɒ񋟂����D
/// �V�[���؂�ւ����̎�i��񋟁D
/// </summary>
class ITopLV
{
public:
	virtual ~ITopLV() = default;

public:
	//�v���C�󋵃f�[�^�ւ̃A�N�Z�X
	virtual PlayData &CurrPlayData() = 0;

public:	//�V�[���J��

	virtual void ChangeToTownScene() = 0;


public:	//BGM
	virtual bool ChangeBGM( int BGM_index ) = 0;
	virtual void StopBMG() = 0;

public:	//Other

	//���b�Z�[�W�{�b�N�X�̃��[�_���\��
	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const = 0;
};

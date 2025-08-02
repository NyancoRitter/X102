#pragma once
#include <string>

class PlayData;

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

	//
	virtual void ChangeToTownScene() = 0;

	//���b�Z�[�W�{�b�N�X�̃��[�_���\��
	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const = 0;
};

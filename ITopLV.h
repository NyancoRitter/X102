#pragma once
#include <string>

/// <summary>
/// �e�V�[���ɒ񋟂����D
/// �V�[���؂�ւ����̎�i��񋟁D
/// </summary>
class ITopLV
{
public:
	virtual ~ITopLV() = default;

public:
	//���b�Z�[�W�{�b�N�X�̃��[�_���\��
	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const = 0;
};

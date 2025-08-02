#pragma once

class IController;

namespace GUI::Menu
{
	class IMenuContent;

	//HandleInput()�̖߂�l�p
	enum class HandleInputResult
	{
		None,	//�L���ȑ��삪�s���Ȃ������i�����ω������j
		CursorMoved,	//�J�[�\���ʒu���ω�����
		Selected,	//���ڑI�𑀍삪�����ꂽ
		Canceled	//�L�����Z�����삪�����ꂽ
	};

	/// <summary>
	/// ���j���[�p�̈�ʓI�ȑ�����͏���
	/// </summary>
	/// <param name="TgtContent">����Ώ�</param>
	/// <param name="Controller">�������</param>
	/// <returns>��������</returns>
	HandleInputResult HandleInput( IMenuContent &TgtContent, const IController &Controller );
}

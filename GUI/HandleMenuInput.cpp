#include "HandleMenuInput.h"
#include "IMenuContent.h"
#include "IController.h"

namespace GUI::Menu
{
	HandleInputResult HandleInput( IMenuContent &TgtContent, const IController &Controller )
	{
		if( TgtContent.nItems() >= 1 )
		{
			//�J�[�\���ړ�
			if( TgtContent.IsVerticalMenu()  ?  Controller.CursorUp()  :  Controller.CursorLeft() )
			{
				if( TgtContent.DecCursorPos() )
				{	return HandleInputResult::CursorMoved;	}
			}

			if( TgtContent.IsVerticalMenu()  ?  Controller.CursorDown()  :  Controller.CursorRight() )
			{
				if( TgtContent.IncCursorPos() )
				{	return HandleInputResult::CursorMoved;	}
			}

			//�I��
			if( Controller.Select()  &&  TgtContent.CursorPos() >= 0 )
			{	return HandleInputResult::Selected;	}
		}

		//�L�����Z��
		if( Controller.Cancel() )
		{	return HandleInputResult::Canceled;	}

		return HandleInputResult::None;
	}
}

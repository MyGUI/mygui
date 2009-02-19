/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"
#include "MMyGUI_DDContainer.h"

MMYGUI_BEGIN_NAMESPACE

public ref class ItemBox : public DDContainer
{

	//--------------------------------------------------------------------
	// объявление типов и конструкторов
	MMYGUI_DECLARE_DERIVED( ItemBox, ItemBox, DDContainer );

	#include "../MMyGUI_GetItemDataAt.h"

	//InsertPoint

};

MMYGUI_END_NAMESPACE

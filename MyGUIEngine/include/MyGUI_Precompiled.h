/*!
	@file
	@author		Alexander Ptakhin
	@date		01/2009
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_PRECOMPILED_H__
#define __MYGUI_PRECOMPILED_H__

#ifdef MYGUI_CHECK_MEMORY_LEAKS
#	if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#		define _CRTDBG_MAP_ALLOC
#	endif
#endif

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		include <stdlib.h>
#		include <crtdbg.h>
#		ifdef _DEBUG
#			define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#			define new DEBUG_NEW
#		endif
#	endif
#endif

#include "MyGUI_Macros.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Types.h"
#include "MyGUI_Any.h"
#include "MyGUI_IResource.h"
#include "MyGUI_Utility.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"
#include "MyGUI_Version.h"
#include "MyGUI_WidgetStyle.h"
#include "MyGUI_ActionController.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_ControllerPosition.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IObject.h"
#include "MyGUI_ISerializable.h"
#include "MyGUI_GenericFactory.h"

#endif // __MYGUI_PRECOMPILED_H__

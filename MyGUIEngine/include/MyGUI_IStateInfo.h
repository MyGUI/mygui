/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_STATE_INFO_H__
#define __MYGUI_I_STATE_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISerializable.h"

namespace MyGUI
{

	class MYGUI_EXPORT IStateInfo :
		public ISerializable
	{
		MYGUI_RTTI_DERIVED( IStateInfo )

	public:
		virtual ~IStateInfo() { }
	};

	typedef std::vector<IStateInfo*> VectorStateInfo;
	typedef std::map<std::string, IStateInfo*> MapStateInfo;
	typedef std::map<std::string, VectorStateInfo> MapWidgetStateInfo;

} // namespace MyGUI

#endif // __MYGUI_I_STATE_INFO_H__

/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_CHILD_SKIN_INFO_H__
#define __MYGUI_CHILD_SKIN_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct _MyGUIExport ChildSkinInfo
	{
		ChildSkinInfo() : align(ALIGN_DEFAULT) { }

		ChildSkinInfo(const std::string& _type, const std::string& _skin, const std::string& _name, const IntCoord& _coord, Align _align, const std::string& _layer) :
			type(_type),
			skin(_skin),
			name(_name),
			layer(_layer),
			coord(_coord),
			align(_align)
		{
		}

		inline void addParam(const std::string& _key, const std::string& _value)
		{
			params[_key] = _value;
		}

		inline std::string findValue(const std::string& _key) const
		{
			MapString::const_iterator iter = params.find(_key);
			if (iter != params.end()) return iter->second;
			return "";
		}

		std::string type, skin, name, layer;
		IntCoord coord;
		Align align;
		MapString params;
	};

	typedef std::vector<ChildSkinInfo> VectorChildSkinInfo;

} // namespace MyGUI


#endif // __MYGUI_CHILD_SKIN_INFO_H__

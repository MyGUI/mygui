/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "ExportMarshaling.h"
#include <algorithm>

namespace Export
{

	MyGUI::IntSize Convert< MyGUI::IntSize >::mHolder;
	MyGUI::IntPoint Convert< MyGUI::IntPoint >::mHolder;
	MyGUI::IntCoord Convert< MyGUI::IntCoord >::mHolder;
	MyGUI::IntRect Convert< MyGUI::IntRect >::mHolder;

	MyGUI::FloatSize Convert< MyGUI::FloatSize >::mHolder;
	MyGUI::FloatPoint Convert< MyGUI::FloatPoint >::mHolder;
	MyGUI::FloatCoord Convert< MyGUI::FloatCoord >::mHolder;
	MyGUI::FloatRect Convert< MyGUI::FloatRect >::mHolder;

	MyGUI::Colour Convert< MyGUI::Colour >::mHolder;

	std::string Convert< std::string >::mHolder;

	const wchar_t* allocateWideString(const MyGUI::UString& value)
	{
		const auto wide = value.asWStr();
		auto* result = new wchar_t[wide.size() + 1];
		std::copy_n(wide.c_str(), wide.size() + 1, result);
		return result;
	}

	MYGUIEXPORT void MYGUICALL ExportMarshaling_FreeWideString(const wchar_t* value)
	{
		delete[] value;
	}

}

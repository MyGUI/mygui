/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "ExportMarshaling.h"

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

	MyGUI::UString Convert< MyGUI::UString >::mHolder;
	std::string Convert< std::string >::mHolder;

}

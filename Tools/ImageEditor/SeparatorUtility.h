/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _1e30e3b7_0116_4490_bed6_83b7ec7cfb21_
#define _1e30e3b7_0116_4490_bed6_83b7ec7cfb21_

#include "MyGUI.h"

namespace tools
{
	inline bool sortHorizontal(MyGUI::Widget* _first, MyGUI::Widget* _second)
	{
		return (_first->getLeft() < _second->getLeft());
	}

	inline bool sortVertical(MyGUI::Widget* _first, MyGUI::Widget* _second)
	{
		return (_first->getTop() < _second->getTop());
	}

	struct SeparatorData
	{
		SeparatorData() :
			FirstPart(nullptr),
			FirstSeparator(nullptr),
			SecondPart(nullptr),
			SecondSeparator(nullptr)
		{
		}

		MyGUI::Widget* FirstPart;
		MyGUI::Widget* FirstSeparator;
		MyGUI::Widget* SecondPart;
		MyGUI::Widget* SecondSeparator;
		//MyGUI::IntPoint PressedPoint;
		MyGUI::IntPoint OffsetPoint;
	};
}

#endif

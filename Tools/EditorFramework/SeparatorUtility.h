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
			PreviousPart(nullptr),
			PreviousSeparator(nullptr),
			NextPart(nullptr),
			NextSeparator(nullptr)
		{
		}

		MyGUI::Widget* PreviousPart;
		MyGUI::Widget* PreviousSeparator;
		MyGUI::Widget* NextPart;
		MyGUI::Widget* NextSeparator;
		MyGUI::IntPoint OffsetPoint;
	};

	enum SeparatorDirection
	{
		Previous,
		Next,
		Any
	};

}

#endif

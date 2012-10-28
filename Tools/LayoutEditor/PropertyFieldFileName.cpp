/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldFileName.h"
#include "Parse.h"

namespace tools
{

	PropertyFieldFileName::PropertyFieldFileName(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldFileName::~PropertyFieldFileName()
	{
	}

	bool PropertyFieldFileName::onCheckValue()
	{
		bool success = utility::checkParseFileName(mField);

		return success;
	}

}

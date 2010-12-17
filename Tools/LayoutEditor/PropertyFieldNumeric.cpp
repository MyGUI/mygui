/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldNumeric.h"
#include "Parse.h"

namespace tools
{

	PropertyFieldNumeric::PropertyFieldNumeric(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldNumeric::~PropertyFieldNumeric()
	{
	}

	bool PropertyFieldNumeric::onCheckValue()
	{
		bool success = true;

		if ("1 int" == mType)
			success = utility::checkParse<int>(mField, 1);
		else if ("2 int" == mType)
			success = utility::checkParse<int>(mField, 2);
		else if ("4 int" == mType)
			success = utility::checkParse<int>(mField, 4);
		else if ("1 float" == mType)
			success = utility::checkParse<float>(mField, 1);
		else if ("2 float" == mType)
			success = utility::checkParse<float>(mField, 2);

		return success;
	}

} // namespace tools

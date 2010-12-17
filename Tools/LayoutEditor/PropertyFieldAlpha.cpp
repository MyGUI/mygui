/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldAlpha.h"
#include "Parse.h"

namespace tools
{

	PropertyFieldAlpha::PropertyFieldAlpha(MyGUI::Widget* _parent) :
		PropertyFieldEditBox(_parent)
	{
	}

	PropertyFieldAlpha::~PropertyFieldAlpha()
	{
	}

	bool PropertyFieldAlpha::onCheckValue()
	{
		bool success = true;

		success = utility::checkParseInterval<float>(mField, 1, 0., 1.);

		return success;
	}

} // namespace tools

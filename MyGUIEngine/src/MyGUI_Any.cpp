/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	const Any Any::Null{};

	Any::Any() = default;

	Any::Any(const Any& other) :
		mContent(other.mContent ? other.mContent->clone() : nullptr)
	{
	}

	Any::Any(Any&& other) noexcept = default;

	Any::~Any() = default;

	Any& Any::operator=(const Any& rhs)
	{
		mContent = rhs.mContent ? rhs.mContent->clone() : nullptr;
		return *this;
	}

	Any& Any::operator=(Any&& rhs) noexcept = default;

	bool Any::empty() const
	{
		return !mContent;
	}

	const std::type_info& Any::getType() const
	{
		return mContent ? mContent->getType() : typeid(void);
	}

	bool Any::compare(const Any& other) const
	{
		if (mContent == nullptr && other.mContent == nullptr)
			return true;
		return mContent != nullptr && other.mContent != nullptr && mContent->compare(other.mContent);
	}

} // namespace MyGUI

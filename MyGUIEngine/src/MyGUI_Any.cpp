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
		mData(other.mData ? other.mType->copy(other.mData) : nullptr),
		mType(other.mType)
	{
	}

	Any::Any(Any&& other) noexcept :
		mData(std::exchange(other.mData, nullptr)),
		mType(std::exchange(other.mType, nullptr))
	{
	}

	Any::~Any()
	{
		if (mData)
			mType->destroy(mData);
	}

	Any& Any::operator=(const Any& rhs)
	{
		return *this = Any(rhs);
	}

	Any& Any::operator=(Any&& rhs) noexcept
	{
		if (this != &rhs)
		{
			// rhs may be nested inside our current value; capture it before destroying that value.
			Any incoming(std::move(rhs));
			std::swap(mData, incoming.mData);
			std::swap(mType, incoming.mType);
		}
		return *this;
	}

	bool Any::empty() const
	{
		return !mData;
	}

	const std::type_info& Any::getType() const
	{
		return mType ? mType->type : typeid(void);
	}

	bool Any::compare(const Any& other) const
	{
		if (empty() || other.empty())
			return empty() == other.empty();
		return mType->compare(mData, other.mData, other.mType->type);
	}

} // namespace MyGUI

/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	Any::AnyEmpty Any::Null;

	Any::Any() :
		mContent(nullptr)
	{
	}

	Any::Any(const Any::AnyEmpty& value) :
		mContent(nullptr)
	{
	}

	Any::Any(const Any& other) :
		mContent(other.mContent ? other.mContent->clone() : nullptr)
	{
	}

	Any::~Any()
	{
		delete mContent;
	}

	Any& Any::swap(Any& rhs)
	{
		std::swap(mContent, rhs.mContent);
		return *this;
	}

	Any& Any::operator = (const Any::AnyEmpty& rhs)
	{
		delete mContent;
		mContent = nullptr;
		return *this;
	}

	Any& Any::operator = (const Any& rhs)
	{
		Any(rhs).swap(*this);
		return *this;
	}

	bool Any::empty() const
	{
		return !mContent;
	}

	const std::type_info& Any::getType() const
	{
		return mContent ? mContent->getType() : typeid(void);
	}

#if defined(__clang__)
	// That's the point of unsafe cast
	__attribute__((no_sanitize("vptr")))
#endif
	void* Any::castUnsafe() const
	{
		return mContent ? static_cast<Any::Holder<void*> *>(this->mContent)->held : nullptr;
	}

	bool Any::compare(const Any& other) const
	{
		if (mContent == nullptr && other.mContent == nullptr)
			return true;
		return mContent != nullptr && other.mContent != nullptr && mContent->compare(other.mContent);
	}

} // namespace MyGUI

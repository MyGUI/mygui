/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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

	void* Any::castUnsafe() const
	{
		return mContent ? static_cast<Any::Holder<void*> *>(this->mContent)->held : nullptr;
	}

} // namespace MyGUI

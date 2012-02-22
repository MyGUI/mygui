/*!
	@file
	@author		Albert Semenov
	@date		05/2009
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
#include "MyGUI_Exception.h"
#include "MyGUI_StringUtility.h"

namespace MyGUI
{

	Exception::Exception(const std::string& _description, const std::string& _source, const char* _file, long _line ) :
		mDescription(_description),
		mSource(_source),
		mFile(_file),
		mLine(_line)
	{
	}

	Exception::Exception(const Exception& _rhs) :
		mDescription(_rhs.mDescription),
		mSource(_rhs.mSource),
		mFile(_rhs.mFile),
		mLine(_rhs.mLine),
		mFullDesc(_rhs.mFullDesc)
	{
	}

	Exception::~Exception() throw()
	{
	}

	Exception& Exception::operator = (const Exception& _rhs)
	{
		mDescription = _rhs.mDescription;
		mSource = _rhs.mSource;
		mFile = _rhs.mFile;
		mLine = _rhs.mLine;
		mFullDesc = _rhs.mFullDesc;
		return *this;
	}

	const std::string& Exception::getFullDescription() const
	{
		if (mFullDesc.empty())
		{
			if ( mLine > 0 )
			{
				mFullDesc = utility::toString("MyGUI EXCEPTION : ", mDescription, " in ", mSource, " at ", mFile, " (line ", mLine, ")");
			}
			else
			{
				mFullDesc = utility::toString("MyGUI EXCEPTION : ", mDescription, " in ", mSource);
			}
		}

		return mFullDesc;
	}

	const std::string& Exception::getSource() const
	{
		return mSource;
	}

	const std::string& Exception::getFile() const
	{
		return mFile;
	}

	long Exception::getLine() const
	{
		return mLine;
	}

	const std::string& Exception::getDescription() const
	{
		return mDescription;
	}

	// Override std::exception::what
	const char* Exception::what() const throw()
	{
		return getFullDescription().c_str();
	}

} // namespace MyGUI

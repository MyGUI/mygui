/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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

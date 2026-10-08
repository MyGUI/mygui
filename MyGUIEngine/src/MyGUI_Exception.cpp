/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Exception.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_StringUtility.h"

#include <utility>

namespace MyGUI
{

	void detail::throwBadCast(std::string_view _from, std::string_view _to, const char* _file, int _line)
	{
		std::ostringstream stream;
		stream << "Cannot cast from type '" << _from << "' to type '" << _to << "'.";
		std::string message = stream.str();
		LogManager::getInstance().log(MYGUI_LOG_SECTION, LogLevel::Critical, message, _file, _line);
		message += '\n';
		throw Exception(std::move(message), "MyGUI", _file, _line);
	}

	Exception::Exception(std::string _description, std::string _source, std::string _file, long _line) :
		mDescription(std::move(_description)),
		mSource(std::move(_source)),
		mFile(std::move(_file)),
		mLine(_line)
	{
	}

	const std::string& Exception::getFullDescription() const
	{
		if (mFullDesc.empty())
		{
			if (mLine > 0)
			{
				mFullDesc = utility::toString(
					"MyGUI EXCEPTION : ",
					mDescription,
					" in ",
					mSource,
					" at ",
					mFile,
					" (line ",
					mLine,
					")");
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
	const char* Exception::what() const noexcept
	{
		return getFullDescription().c_str();
	}

} // namespace MyGUI

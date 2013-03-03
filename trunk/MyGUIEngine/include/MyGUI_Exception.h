/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_EXCEPTION_H__
#define __MYGUI_EXCEPTION_H__

#include "MyGUI_Prerequest.h"
#include <exception>
#include <string>

namespace MyGUI
{

	class MYGUI_EXPORT Exception :
		public std::exception
	{
	public:
		Exception(const std::string& _description, const std::string& _source, const char* _file, long _line);

		Exception(const Exception& _rhs);

		// Needed for  compatibility with std::exception
		~Exception() throw();

		Exception& operator = (const Exception& _rhs);

		virtual const std::string& getFullDescription() const;

		virtual const std::string& getSource() const;

		virtual const std::string& getFile() const;

		virtual long getLine() const;

		virtual const std::string& getDescription() const;

		// Override std::exception::what
		const char* what() const throw();

	protected:
		std::string mDescription;
		std::string mSource;
		std::string mFile;
		long mLine;
		mutable std::string mFullDesc;
	};

} // namespace MyGUI

#endif // __MYGUI_EXCEPTION_H__

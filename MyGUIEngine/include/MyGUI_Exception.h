/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_EXCEPTION_H_
#define MYGUI_EXCEPTION_H_

#include "MyGUI_Prerequest.h"
#include <exception>
#include <string>

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
 // disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
#	pragma warning (push)
#	pragma warning (disable : 4275)
#endif

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

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	pragma warning (pop)
#endif

#endif // MYGUI_EXCEPTION_H_

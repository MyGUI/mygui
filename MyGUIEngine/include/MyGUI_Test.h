/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __MYGUI_TEST_H__
#define __MYGUI_TEST_H__

#include "MyGUI_Prerequest.h"
#include <typeinfo>

namespace MyGUI
{

	class _MyGUIExport Test
	{

	public:

		template<typename T> bool isType() const
		{
			return isType( typeid( T ));
		}

		virtual bool isType( const std::type_info & t) const
		{
			return typeid(Test) == t;
		}

		virtual const std::string & getTypeName()
		{
			return Test::getClassTypeName();
		}

		/*
		struct TestType
		{
			const std::string & getClassTypeName()
			{
				static std::string type = "Test";
				return type;
			}
		};

		static const std::string & getClassTypeName()
		{
			TestType type;
			return type.getClassTypeName();
		}
		*/

		static const std::string & getClassTypeName()
		{
			static std::string type = "Test";
			return type;
		}

		template<typename T> T* castType(bool _throw = true)
		{
			if (this->isType<T>()) return static_cast<T*>( this );
			return null;
		}

		template<typename T> const T* castType(bool _throw = true) const
		{
			if (this->isType<T>()) return static_cast<T*>( this );
			return null;
		}

	};

}

#endif // __MYGUI_TEST_H__

/*!
	@file
	@author		Alexander Buryak
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RTTI_H__
#define __MYGUI_RTTI_H__

#include "MyGUI_Prerequest.h"
#include <typeinfo>

namespace MyGUI
{

	#define MYGUI_RTTI_BASE_HEADER( T ) \
		public: \
			template<typename T> bool isType() const { return isType( typeid( T )); } \
			virtual bool isType( const std::type_info & t) const { return typeid( T ) == t; }	\
			virtual const std::string & getTypeName() { return T::getClassTypeName(); } \
			static const std::string & getClassTypeName() { static std::string type = #T; return type; } \
			template<typename T> T* castType(bool _throw = true) \
			{ \
				if (this->isType<T>()) return static_cast<T*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << T::getClassTypeName() << "' .") \
				return null; \
			} \
			template<typename T> const T* castType(bool _throw = true) const \
			{ \
				if (this->isType<T>()) return static_cast<T*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << T::getClassTypeName() << "' .") \
				return null; \
			}

	#define MYGUI_RTTI_CHILD_HEADER( T, BT ) \
		public: \
			virtual bool isType( const std::type_info &t ) const { return typeid( T ) == t || BT::isType( t ); }	\
			virtual const std::string & getTypeName() { return T::getClassTypeName(); } \
			static const std::string & getClassTypeName() { static std::string type = #T; return type; }

} // namespace MyGUI

#endif // __MYGUI_RTTI_H__

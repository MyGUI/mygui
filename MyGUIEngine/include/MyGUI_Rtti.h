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
#include "MyGUI_Common.h"
#include <typeinfo>

namespace MyGUI
{

	//VC++ 7.1
	#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && MYGUI_COMP_VER == 1310
		#define MYGUI_GET_TYPE_NAME( Type ) \
			struct TypeNameHolder { const std::string & getClassTypeName() { static std::string type = #Type; return type; } }; \
			static const std::string & getClassTypeName() { TypeNameHolder type; return type.getClassTypeName(); }
	#else
		#define MYGUI_GET_TYPE_NAME( Type ) \
			static const std::string & getClassTypeName() { static std::string type = #Type; return type; }
	#endif

	#define MYGUI_RTTI_BASE_HEADER( BaseType ) \
		public: \
			virtual bool isType( const std::type_info & t) const { return typeid( BaseType ) == t; }	\
			virtual const std::string & getTypeName() { return BaseType::getClassTypeName(); } \
			MYGUI_GET_TYPE_NAME( BaseType ) \
			\
			template<typename Type> bool isType() const { return isType( typeid( Type )); } \
			template<typename Type> Type* castType(bool _throw = true) \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' .") \
				return nullptr; \
			} \
			template<typename Type> const Type* castType(bool _throw = true) const \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' .") \
				return nullptr; \
			}

	#define MYGUI_RTTI_CHILD_HEADER( DerivedType, BaseType ) \
		public: \
			virtual bool isType( const std::type_info &t ) const { return typeid( DerivedType ) == t || BaseType::isType( t ); }	\
			virtual const std::string & getTypeName() { return DerivedType::getClassTypeName(); } \
			MYGUI_GET_TYPE_NAME( DerivedType )

} // namespace MyGUI

#endif // __MYGUI_RTTI_H__

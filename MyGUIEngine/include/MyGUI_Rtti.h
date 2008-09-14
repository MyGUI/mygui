/*!
	@file
	@author		Rageous
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RTTI_H__
#define __MYGUI_RTTI_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	#define MYGUI_RTTI_BASE_HEADER \
		public: \
			template<typename T> bool isType() const { return isType( typeid( T )); } \
			virtual bool isType( const type_info & t) const; \
			virtual const std::string & getTypeName(); \
			static const std::string & getClassTypeName(); \
			template<typename T> inline T* castType(bool _throw = true) \
			{ \
				if (this->isType<T>()) return static_cast<T*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << T::getClassTypeName() << "' .") \
				return null; \
			} \
			template<typename T> inline const T* castType(bool _throw = true) const \
			{ \
				if (this->isType<T>()) return static_cast<T*>( this ); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << T::getClassTypeName() << "' .") \
				return null; \
			} \
		private: \
			static std::string mTypeName;

	#define MYGUI_RTTI_BASE_IMPLEMENT( T ) \
		bool T::isType( const type_info & t) const { return typeid( T ) == t; } \
		const std::string & T::getTypeName() { return mTypeName; } \
		const std::string & T::getClassTypeName() { return mTypeName; } \
		std::string T::mTypeName = #T;


	#define MYGUI_RTTI_CHILD_HEADER \
		public: \
			virtual bool isType( const type_info &t ) const; \
			virtual const std::string & getTypeName(); \
			static const std::string & getClassTypeName(); \
		private: \
			static std::string mTypeName;

	#define MYGUI_RTTI_CHILD_IMPLEMENT( T, BT ) \
		bool T::isType( const type_info &t ) const { return typeid( T ) == t || BT::isType( t ); } \
		const std::string & T::getClassTypeName() { return mTypeName; } \
		const std::string & T::getTypeName() { return mTypeName; } \
		std::string T::mTypeName = #T;

} // namespace MyGUI

#endif // __MYGUI_RTTI_H__

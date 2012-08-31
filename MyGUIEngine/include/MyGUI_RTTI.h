/*!
	@file
	@author		Alexander Buryak - Rageous
	@author		Albert Semenov
	@date		09/2008
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
#ifndef __MYGUI_RTTI_H__
#define __MYGUI_RTTI_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Diagnostic.h"
#include <string>

#include <typeinfo>

namespace MyGUI
{
	#define MYGUI_RTTI_TYPE const std::type_info&
	#define MYGUI_RTTI_GET_TYPE(type) typeid(type)

	//VC++ 7.1
	#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && MYGUI_COMP_VER <= 1310
		#define MYGUI_DECLARE_TYPE_NAME(Type) \
		private: \
			struct TypeNameHolder { const std::string& getClassTypeName() { static std::string type = #Type; return type; } }; \
		public: \
			static const std::string& getClassTypeName() { TypeNameHolder type; return type.getClassTypeName(); } \
			/** Get type name as string */ \
			virtual const std::string& getTypeName() const { return getClassTypeName(); }
	#else
		#define MYGUI_DECLARE_TYPE_NAME(Type) \
		public: \
			static const std::string& getClassTypeName() { static std::string type = #Type; return type; } \
			/** Get type name as string */ \
			virtual const std::string& getTypeName() const { return getClassTypeName(); }
	#endif

	#define MYGUI_RTTI_BASE(BaseType) \
		public: \
			typedef BaseType RTTIBase; \
			MYGUI_DECLARE_TYPE_NAME(BaseType) \
			/** Compare with selected type */ \
			virtual bool isType(MYGUI_RTTI_TYPE _type) const { return MYGUI_RTTI_GET_TYPE(BaseType) == _type; } \
			/** Compare with selected type */ \
			template<typename Type> bool isType() const { return isType(MYGUI_RTTI_GET_TYPE(Type)); } \
			/** Try to cast pointer to selected type. \
				@param _throw If true throw exception when casting in wrong type, else return nullptr \
			*/ \
			template<typename Type> Type* castType(bool _throw = true) \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>(this); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' .") \
				return nullptr; \
			} \
			/** Try to cast pointer to selected type. \
				@param _throw If true throw exception when casting in wrong type, else return nullptr \
			*/ \
			template<typename Type> const Type* castType(bool _throw = true) const \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>(this); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' .") \
				return nullptr; \
			}

	#define MYGUI_RTTI_DERIVED(DerivedType) \
		public: \
			MYGUI_DECLARE_TYPE_NAME(DerivedType) \
			typedef RTTIBase Base; \
			typedef DerivedType RTTIBase; \
			/** Compare with selected type */ \
			virtual bool isType(MYGUI_RTTI_TYPE _type) const { return MYGUI_RTTI_GET_TYPE(DerivedType) == _type || Base::isType(_type); } \
			/** Compare with selected type */ \
			template<typename Type> bool isType() const { return isType(MYGUI_RTTI_GET_TYPE(Type)); }

} // namespace MyGUI

#endif // __MYGUI_RTTI_H__

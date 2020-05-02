/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RTTI_H_
#define MYGUI_RTTI_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Diagnostic.h"
#include <string>

#include <typeinfo>

namespace MyGUI
{

#define MYGUI_DECLARE_TYPE_NAME(Type, Override) \
		public: \
			static const std::string& getClassTypeName() { static std::string type = #Type; return type; } \
			/** Get type name as string */ \
			virtual const std::string& getTypeName() const Override { return getClassTypeName(); }

#define MYGUI_RTTI_BASE(BaseType) \
		public: \
			typedef BaseType RTTIBase; \
			MYGUI_DECLARE_TYPE_NAME(BaseType,) \
			/** Compare with selected type */ \
			virtual bool isType(const std::type_info& _type) const { return typeid(BaseType) == _type; } \
			/** Compare with selected type */ \
			template<typename Type> bool isType() const { return isType(typeid(Type)); } \
			/** Try to cast pointer to selected type. \
				@param _throw If true throw exception when casting in wrong type, else return nullptr \
			*/ \
			template<typename Type> Type* castType(bool _throw = true) \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>(this); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' ."); \
				return nullptr; \
			} \
			/** Try to cast pointer to selected type. \
				@param _throw If true throw exception when casting in wrong type, else return nullptr \
			*/ \
			template<typename Type> const Type* castType(bool _throw = true) const \
			{ \
				if (this->isType<Type>()) return static_cast<Type*>(this); \
				MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' ."); \
				return nullptr; \
			}

#define MYGUI_RTTI_DERIVED(DerivedType) \
		public: \
			MYGUI_DECLARE_TYPE_NAME(DerivedType, override) \
			typedef RTTIBase Base; \
			typedef DerivedType RTTIBase; \
			/** Compare with selected type */ \
			virtual bool isType(const std::type_info& _type) const override { return typeid(DerivedType) == _type || Base::isType(_type); } \
			/** Compare with selected type */ \
			template<typename Type> bool isType() const { return isType(typeid(Type)); }

} // namespace MyGUI

#endif // MYGUI_RTTI_H_

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _7f0b8d22_75f3_4772_958b_5d7b1982a44b_
#define _7f0b8d22_75f3_4772_958b_5d7b1982a44b_

#include "DataType.h"
#include <map>
#include <type_traits>
#include "Property.h"

namespace tools
{

	class MYGUI_EXPORT_DLL Data
	{
	public:
		typedef std::shared_ptr<Data> DataPtr;
		typedef std::weak_ptr<Data> DataWeak;

		Data();
		~Data();

		static DataPtr CreateInstance();

		void setType(DataTypePtr _value);
		DataTypePtr getType() const;

		DataPtr getParent();

		typedef std::vector<DataPtr> VectorData;
		const VectorData& getChilds() const;

		void addChild(DataPtr _child);
		void insertChild(size_t _index, DataPtr _child);
		void removeChild(DataPtr _child);

		size_t getChildIndex(DataPtr _child);
		DataPtr getChildByIndex(size_t _index);

		typedef std::map<std::string, PropertyPtr, std::less<>> MapProperty;
		const MapProperty& getProperties() const;

		const std::string& getPropertyValue(std::string_view _name) const;
		void setPropertyValue(std::string_view _name, std::string_view _value) const;

		template <typename Type>
		Type getPropertyValue(std::string_view _name) const
		{
			return MyGUI::utility::parseValue<Type>(getPropertyValue(_name));
		}

		template <class Type, typename = std::enable_if_t<!std::is_convertible_v<Type, std::string_view>>>
		void setPropertyValue(std::string_view _name, const Type& _value) const
		{
			setPropertyValue(_name, MyGUI::utility::toString(_value));
		}

		template <>
		void setPropertyValue(std::string_view _name, const bool& _value) const
		{
			setPropertyValue(_name, _value ? "True" : "False");
		}

		PropertyPtr getProperty(std::string_view _name) const;

		DataPtr getChildSelected();
		void setChildSelected(DataPtr _child);

	private:
		void clear();

	private:
		DataTypePtr mType;
		DataPtr mParent;
		VectorData mChilds;
		MapProperty mProperties;
		size_t mIndexSelected;
		DataWeak mWeakThis;
	};

	typedef Data::DataPtr DataPtr;

}

#endif

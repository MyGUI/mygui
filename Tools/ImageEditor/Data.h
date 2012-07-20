/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _7f0b8d22_75f3_4772_958b_5d7b1982a44b_
#define _7f0b8d22_75f3_4772_958b_5d7b1982a44b_

#include "DataType.h"
#include <map>
#include "Property.h"

namespace tools
{

	class Data
	{
	public:
		Data();
		~Data();

		void setType(DataType* _value);
		DataType* getType();

		Data* getParent();

		typedef std::vector<Data*> VectorData;
		const VectorData& getChilds() const;

		void addChild(Data* _child);
		void insertChild(size_t _index, Data* _child);
		void removeChild(Data* _child);

		size_t getChildIndex(Data* _child);
		Data* getChildByIndex(size_t _index);

		typedef std::map<std::string, Property*> MapString;
		const MapString& getProperties() const;

		const std::string& getPropertyValue(const std::string& _name) const;
		void setPropertyValue(const std::string& _name, const std::string& _value);

		template <typename Type>
		Type getPropertyValue(const std::string& _name) const
		{
			return MyGUI::utility::parseValue<Type>(getPropertyValue(_name));
		}

		template <typename Type>
		void setPropertyValue(const std::string& _name, const Type& _value)
		{
			setPropertyValue(_name, MyGUI::utility::toString(_value));
		}

		template <>
		void setPropertyValue(const std::string& _name, const bool& _value)
		{
			setPropertyValue(_name, std::string(_value ? "True" : "False"));
		}

		Property* getProperty(const std::string& _name) const;

		Data* getChildSelected();
		void setChildSelected(Data* _child);

	private:
		void clear();

	private:
		DataType* mType;
		Data* mParent;
		VectorData mChilds;
		MapString mProperties;
		size_t mIndexSelected;
	};

}

#endif

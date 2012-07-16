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
		const MapString getProperties() const;

		const std::string& getPropertyValue(const std::string& _name) const;
		void setPropertyValue(const std::string& _name, const std::string& _value);

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

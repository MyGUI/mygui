/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _7f0b8d22_75f3_4772_958b_5d7b1982a44b_
#define _7f0b8d22_75f3_4772_958b_5d7b1982a44b_

#include "DataInfo.h"
#include <MyGUI.h>
#include <map>

namespace tools
{
	class Data
	{
	public:
		Data();
		~Data();

		void setType(DataInfo* _value);
		DataInfo* getType();

		Data* getParent();

		typedef std::vector<Data*> VectorData;
		const VectorData& getChilds() const;

		void addChild(Data* _child);
		void removeChild(Data* _child);

		const MyGUI::MapString getProperties() const;

		const std::string& getPropertyValue(const std::string& _name) const;
		void setPropertyValue(const std::string& _name, const std::string& _value);

	private:
		void clear();

	private:
		DataInfo* mType;
		Data* mParent;
		VectorData mChilds;
		MyGUI::MapString mProperties;
	};
}

#endif

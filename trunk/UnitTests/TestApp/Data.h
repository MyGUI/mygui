/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _7f0b8d22_75f3_4772_958b_5d7b1982a44b_
#define _7f0b8d22_75f3_4772_958b_5d7b1982a44b_

#include "DataInfo.h"
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

		void AddChild(Data* _child);
		void RemoveChild(Data* _child);

		typedef std::map<std::string, std::string> MapString;
		const MapString getProperties() const;

	private:
		void clear();

	private:
		DataInfo* mType;
		Data* mParent;
		VectorData mChilds;
		MapString mProperties;
	};
}

#endif

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _dd541c2d_6b2d_440f_a2b3_a08454979c7b_
#define _dd541c2d_6b2d_440f_a2b3_a08454979c7b_

#include "pugixml.hpp"
#include <vector>
#include "DataPropertyInfo.h"

namespace tools
{
	class DataInfo
	{
	public:
		DataInfo();
		~DataInfo();

		void deserialization(pugi::xml_node _node);

		const std::string& getType() const;

		typedef std::vector<std::string> VectorString;
		const VectorString& getChilds() const;

		typedef std::vector<DataPropertyInfo*> VectorProperty;
		const VectorProperty& getProperties() const;

	private:
		std::string mType;
		VectorString mChilds;
		VectorProperty mProperties;
	};
}

#endif

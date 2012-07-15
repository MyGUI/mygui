/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _dd541c2d_6b2d_440f_a2b3_a08454979c7b_
#define _dd541c2d_6b2d_440f_a2b3_a08454979c7b_

#include "pugixml.hpp"
#include <vector>
#include "DataTypeProperty.h"

namespace tools
{

	class DataType
	{
	public:
		DataType();
		~DataType();

		void deserialization(pugi::xml_node _node);

		const std::string& getName() const;

		typedef std::vector<std::string> VectorString;
		const VectorString& getChilds() const;

		typedef std::vector<DataTypeProperty*> VectorProperty;
		const VectorProperty& getProperties() const;

		bool isChild(DataType* _child);

	private:
		std::string mName;
		VectorString mChilds;
		VectorProperty mProperties;
	};

}

#endif

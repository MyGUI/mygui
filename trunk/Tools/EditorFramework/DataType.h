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
#include "SharedPtr.h"

namespace tools
{

	class MYGUI_EXPORT_DLL DataType
	{
	public:
		DataType();
		~DataType();

		void deserialization(pugi::xml_node _node);

		const std::string& getName() const;
		const std::string& getFriend() const;

		typedef std::vector<std::string> VectorString;
		const VectorString& getChilds() const;

		typedef std::vector<DataTypePropertyPtr> VectorProperty;
		const VectorProperty& getProperties() const;

		bool isChild(const std::string& _child) const;

	private:
		std::string mName;
		std::string mFriend;
		VectorString mChilds;
		VectorProperty mProperties;
	};

	typedef shared_ptr<DataType> DataTypePtr;

}

#endif

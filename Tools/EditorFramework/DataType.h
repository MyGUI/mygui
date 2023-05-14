/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _dd541c2d_6b2d_440f_a2b3_a08454979c7b_
#define _dd541c2d_6b2d_440f_a2b3_a08454979c7b_

#include "pugixml.hpp"
#include <memory>
#include <vector>
#include "DataTypeProperty.h"

namespace tools
{

	class MYGUI_EXPORT_DLL DataType
	{
	public:
		void deserialization(pugi::xml_node _node);

		const std::string& getName() const;
		const std::string& getFriend() const;

		using VectorString = std::vector<std::string>;
		const VectorString& getChilds() const;

		using VectorProperty = std::vector<DataTypePropertyPtr>;
		const VectorProperty& getProperties() const;

		bool isChild(std::string_view _child) const;

	private:
		std::string mName;
		std::string mFriend;
		VectorString mChilds;
		VectorProperty mProperties;
	};

	using DataTypePtr = std::shared_ptr<DataType>;

}

#endif

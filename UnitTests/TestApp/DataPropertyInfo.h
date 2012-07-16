/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _65c9f20e_be2b_444e_bd60_f2025ce3e0f0_
#define _65c9f20e_be2b_444e_bd60_f2025ce3e0f0_

#include "pugixml.hpp"
#include <vector>

namespace tools
{
	class DataPropertyInfo
	{
	public:
		DataPropertyInfo();
		~DataPropertyInfo();

		void deserialization(pugi::xml_node _node);

		const std::string& getName() const;
		const std::string& getType() const;

	private:
		std::string mName;
		std::string mType;
	};
}

#endif

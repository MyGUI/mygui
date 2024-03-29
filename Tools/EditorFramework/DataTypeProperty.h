/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _65c9f20e_be2b_444e_bd60_f2025ce3e0f0_
#define _65c9f20e_be2b_444e_bd60_f2025ce3e0f0_

#include "pugixml.hpp"
#include <memory>
#include <vector>

namespace tools
{

	class MYGUI_EXPORT_DLL DataTypeProperty
	{
	public:
		void deserialization(pugi::xml_node _node);

		const std::string& getName() const;
		const std::string& getType() const;
		const std::string& getDefaultValue() const;
		const std::string& getInitialisator() const;
		bool getReadOnly() const;
		bool getVisible() const;
		const std::string& getAction() const;

	private:
		std::string mName;
		std::string mType;
		std::string mDefaultValue;
		std::string mInitialisator;
		bool mReadOnly{false};
		bool mVisible{false};
		std::string mAction;
	};

	using DataTypePropertyPtr = std::shared_ptr<DataTypeProperty>;

}

#endif

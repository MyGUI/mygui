/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _2763d275_35ab_4f13_88a7_e6cd0b2c6430_
#define _2763d275_35ab_4f13_88a7_e6cd0b2c6430_

#include "IFactory.h"
#include "FactoryTemplate.h"
#include <string>
#include <string_view>

namespace factories
{
	bool MYGUI_EXPORT_DLL IsExistFactoryName(std::string_view _factoryName);
	void MYGUI_EXPORT_DLL RegisterFactory(::components::IFactory* _factory, std::string_view _factoryName);

	template<typename Type>
	class FactoryItemRegistrator
	{
	public:
		FactoryItemRegistrator(std::string_view _factoryName) :
			mFactoryName(_factoryName)
		{
			if (!IsExistFactoryName(mFactoryName))
			{
				::components::IFactory* factory = new components::FactoryTemplate<Type>();
				RegisterFactory(factory, mFactoryName);
			}
		}

	private:
		std::string mFactoryName;
	};
}

#endif

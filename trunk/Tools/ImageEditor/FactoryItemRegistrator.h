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

namespace factories
{
	bool IsExistFactoryName(const std::string& _factoryName);
	void RegisterFactory(::components::IFactory* _factory, const std::string& _factoryName);
	void UnregisterFactory(::components::IFactory* _factory, const std::string& _factoryName);

	template <typename Type>
	class FactoryItemRegistrator
	{
	public:
		FactoryItemRegistrator(const std::string& _factoryName) :
			mFactory(0),
			mFactoryName(_factoryName)
		{
			if (!IsExistFactoryName(mFactoryName))
			{
				mFactory = new components::FactoryTemplate<Type>();
				RegisterFactory(mFactory, mFactoryName);
			}
		}
		~FactoryItemRegistrator()
		{
			if (mFactory != 0)
			{
				UnregisterFactory(mFactory, mFactoryName);
				delete mFactory;
				mFactory = 0;
			}
		}

	private:
		FactoryItemRegistrator() { }

	private:
		::components::IFactory* mFactory;
		std::string mFactoryName;
	};
}

#endif

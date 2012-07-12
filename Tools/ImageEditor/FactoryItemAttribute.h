/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _974be190_ed90_49c5_a20f_700745d10220_
#define _974be190_ed90_49c5_a20f_700745d10220_

#define FACTORY_ITEM_ATTRIBUTE(name) \
class name; \
namespace factories \
{ \
	namespace \
	{ \
		class Registrator \
		{ \
		public: \
			Registrator() : \
				mFactory(0) \
			{ \
				if (!::components::FactoryManager::GetInstancePtr()->ExistFactory(#name)) \
				{ \
					mFactory = new ::components::FactoryTemplate<name>(); \
					::components::FactoryManager::GetInstancePtr()->RegisterFactory(mFactory, #name); \
				} \
			} \
			~Registrator() \
			{ \
				if (mFactory != 0) \
				{ \
					::components::FactoryManager::GetInstancePtr()->UnregisterFactory(mFactory, #name); \
					delete mFactory; \
					mFactory = 0; \
				} \
			} \
		private: \
			::components::IFactory* mFactory; \
		}; \
		static Registrator instance; \
	} \
}

#endif

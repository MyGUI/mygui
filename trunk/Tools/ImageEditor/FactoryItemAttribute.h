#pragma once

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

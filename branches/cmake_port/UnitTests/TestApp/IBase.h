/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_BASE_H__
#define __I_BASE_H__

#include <MyGUI.h>

namespace sim
{

	class IBase
	{
	public:
		MYGUI_RTTI_BASE_HEADER(IBase);

		template<typename Type> Type* queryType(bool _throw = true)
		{
			// пробуем себя сначала
			Type * type = this->castType<Type>(false);
			if (type) return type;

			// потом всех остальных
			for (size_t index=0; index<mBaseItems.size(); ++index) {
				Type * type = mBaseItems[index]->castType<Type>(false);
				if (type) return type;
			}

			MYGUI_ASSERT(!_throw, "Error cast type '" << this->getTypeName() << "' to type '" << Type::getClassTypeName() << "' .")
			return nullptr;
		}

		void addBase(IBase * _base)
		{
			for (size_t index=0; index<mBaseItems.size(); ++index) {
				mBaseItems[index]->mBaseItems.push_back(_base);
			}
			_base->mBaseItems = mBaseItems;
			_base->mBaseItems.push_back(this);
			mBaseItems.push_back(_base);
		}

		// для инициализации
		void initialiseBase()
		{
			for (size_t index=0; index<mBaseItems.size(); ++index) {
				mBaseItems[index]->initialise();
			}
		}

	protected:
		virtual void initialise()
		{
		}

	private:
		typedef std::vector<IBase*> VectorBase;
		VectorBase mBaseItems;
	};

} // namespace sim

#endif // __I_BASE_H__

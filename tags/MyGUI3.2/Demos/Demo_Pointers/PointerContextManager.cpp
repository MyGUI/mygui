/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#include "Precompiled.h"
#include "PointerContextManager.h"

namespace demo
{

	PointerContextManager::PointerContextManager(IPointerSetter* _setter) :
		mPointerSetter(_setter)
	{
		mCurrentGuiPointer = MyGUI::PointerManager::getInstance().getDefaultPointer();
		MyGUI::PointerManager::getInstance().eventChangeMousePointer += MyGUI::newDelegate(this, &PointerContextManager::notifyChangeMousePointer);
	}

	void PointerContextManager::notifyChangeMousePointer(const std::string& _name)
	{
		mCurrentGuiPointer = _name;
		updateCursor();
	}

	bool PointerContextManager::isHighLevelContext() const
	{
		if (!mContexts.empty())
			return mContexts.back()->isHighLevel();
		return false;
	}

	void PointerContextManager::addContext(const std::string& _name)
	{
		bool found = false;
		for (VectorContext::iterator item = mContexts.begin(); item != mContexts.end(); ++item)
		{
			if (_name == (*item)->getResourceName())
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			MyGUI::IResource* resource_generic = MyGUI::ResourceManager::getInstance().getByName(_name, false);
			if (resource_generic != nullptr)
			{
				ResourcePointerContext* resource = resource_generic->castType<ResourcePointerContext>(false);
				if (resource != nullptr)
				{
					if (!isHighLevelContext())
					{
						mContexts.push_back(resource);
					}
					else
					{
						// вставляем перед первым хаем
						for (VectorContext::iterator item = mContexts.begin(); item != mContexts.end(); ++item)
						{
							if ((*item)->isHighLevel())
							{
								mContexts.insert(item, resource);
								break;
							}
						}
					}

				}
			}
		}

		updateCursor();
	}

	void PointerContextManager::removeContext(const std::string& _name)
	{
		for (VectorContext::iterator item = mContexts.begin(); item != mContexts.end(); ++item)
		{
			if (_name == (*item)->getResourceName())
			{
				mContexts.erase(item);
				break;
			}
		}

		updateCursor();
	}

	void PointerContextManager::setPointer(const std::string& _name)
	{
		if (mCurrentPointer != _name)
		{
			mCurrentPointer = _name;
			updateCursor();
		}
	}

	void PointerContextManager::updateCursor()
	{
		if (isHighLevelContext())
		{
			_setPointer("default");
		}
		else
		{
			if (mCurrentGuiPointer != MyGUI::PointerManager::getInstance().getDefaultPointer())
			{
				mPointerSetter->setPointer(mCurrentGuiPointer);
			}
			else
			{
				_setPointer(mCurrentPointer);
			}
		}
	}

	void PointerContextManager::_setPointer(const std::string& _name)
	{
		if (!mContexts.empty())
		{
			std::string pointer = mContexts.back()->getPointer(_name);
			if (!pointer.empty())
			{
				mPointerSetter->setPointer(pointer);
			}
		}
	}

} // namespace demo

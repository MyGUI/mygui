/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#include "precompiled.h"
#include "PointerManager.h"

namespace demo
{

	PointerManager::PointerManager()
	{
		mCurrentGuiPointer = MyGUI::PointerManager::getInstance().getDefaultPointer();
		MyGUI::PointerManager::getInstance().eventChangeMousePointer = MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);
	}

	void PointerManager::notifyChangeMousePointer(const std::string& _name)
	{
		mCurrentGuiPointer = _name;
		updateCursor();
	}

	bool PointerManager::isHighLevelContext()
	{
		if (!mContexts.empty())
			return mContexts.back()->isHighLevel();
		return false;
	}

	void PointerManager::addContext(const std::string& _name)
	{
		bool found = false;
		for (VectorContext::iterator item=mContexts.begin(); item!=mContexts.end(); ++item)
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
						for (VectorContext::iterator item=mContexts.begin(); item!=mContexts.end(); ++item)
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

	void PointerManager::removeContext(const std::string& _name)
	{
		for (VectorContext::iterator item=mContexts.begin(); item!=mContexts.end(); ++item)
		{
			if (_name == (*item)->getResourceName())
			{
				mContexts.erase(item);
				break;
			}
		}

		updateCursor();
	}

	void PointerManager::setPointer(const std::string& _name)
	{
		 if (mCurrentPointer != _name)
		 {
			 mCurrentPointer = _name;
			 updateCursor();
		 }
	}

	void PointerManager::updateCursor()
	{
		if (isHighLevelContext())
		{
			_setPointer("normal");
		}
		else
		{
			if (mCurrentGuiPointer != MyGUI::PointerManager::getInstance().getDefaultPointer())
			{
				MyGUI::PointerManager::getInstance().setPointer(mCurrentGuiPointer);
			}
			else
			{
				_setPointer(mCurrentPointer);
			}
		}
	}

	void PointerManager::_setPointer(const std::string& _name)
	{
		 if (!mContexts.empty())
		 {
			 std::string pointer = mContexts.back()->getPointer(_name);
			 if (!pointer.empty())
			 {
				 MyGUI::PointerManager::getInstance().setPointer(pointer);
			 }
		 }
	}

} // namespace demo

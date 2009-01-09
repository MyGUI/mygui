/*!
@file
@author		Georgiy Evmenov
@date		08/2008
@module
*/
#include "MyGUI_DelegateManager.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(DelegateManager);

	void DelegateManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		// nothing yet

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void DelegateManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		// nothing yet

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void DelegateManager::addDelegate(const Ogre::String & _key, HandleEvent::IDelegate * _delegate)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) {
			MYGUI_LOG(Warning, "Delegate '" << _key << "' already exist");
		}
		mDelegates[_key] = _delegate;
	}

	void DelegateManager::removeDelegate(const std::string & _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter == mDelegates.end()) {
			MYGUI_LOG(Warning, "Delegate '" << _key << "' not found");
		}
		mDelegates.erase(iter);
	}

	void DelegateManager::callDelegate(WidgetPtr _sender, const std::string & _key, const std::string & _event)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) {
			iter->second(_sender, _key, _event);
		}
		else
		{
			MYGUI_LOG(Warning, "Delegate '" << _key << "' not found");
		}
	}

} // namespace MyGUI

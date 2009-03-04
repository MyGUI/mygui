/*!
@file
@author		Georgiy Evmenov
@date		08/2008
@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_DelegateManager.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(DelegateManager);

	void DelegateManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mDefaultDelegate = nullptr;

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

	void DelegateManager::addDefaultDelegate(HandleEvent::IDelegate * _delegate)
	{
		mDefaultDelegate = _delegate;
	}

	void DelegateManager::callDelegate(WidgetPtr _sender, const std::string & _key, const std::string & _event)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) {
			iter->second(_sender, _key, _event);
		}
		else
		{
			if (mDefaultDelegate.empty())
			{
				mDefaultDelegate(_sender, _key, _event);
			}
			else
			{
				MYGUI_LOG(Warning, "Delegate '" << _key << "' not found");
			}
		}
	}

} // namespace MyGUI

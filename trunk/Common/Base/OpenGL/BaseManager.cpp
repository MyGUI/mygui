/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
*/
/*
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
#include "precompiled.h"
#include "BaseManager.h"

namespace base
{

	BaseManager * BaseManager::m_instance = nullptr;
	BaseManager & BaseManager::getInstance()
	{
		assert(m_instance);
		return *m_instance;
	}

	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr)
	{
		assert(!m_instance);
		m_instance = this;
	}

	BaseManager::~BaseManager()
	{
		m_instance = nullptr;
	}

	void BaseManager::createInput() // создаем систему ввода
	{
	}

	void BaseManager::destroyInput() // удаляем систему ввода
	{
	}

	bool BaseManager::create()
	{
		createInput();
		createGui();
		createScene();

		return true;
	}

	void BaseManager::run()
	{
	}

	void BaseManager::destroy()
	{
		destroyScene();
		destroyGui();
		destroyInput();
	}

	void BaseManager::createGui()
	{
		/*mPlatform = new MyGUI::DirectXPlatform();
		mPlatform->initialise(device);

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise();

		mInfo = new statistic::StatisticInfo();*/
	}

	void BaseManager::destroyGui()
	{
		/*if (mGUI)
		{
			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}

			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}*/
	}

	void BaseManager::setWindowCaption(const std::string & _text)
	{
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
	}

	void BaseManager::setDescriptionText(const MyGUI::UString & _text)
	{
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, bool _recursive)
	{
	}

	void BaseManager::setupResources()
	{
		//addResourceLocation(mRootMedia, false);
		//addResourceLocation(mRootMedia + "/MyGUI_Media", false);
	}	

} // namespace base

/*!
	@file		DynLibManager.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "DynLibManager.h"


namespace MyGUI
{

	DynLibManager* DynLibManager::m_instance = 0;

	DynLibManager::DynLibManager()
	{

	}

	DynLibManager::~DynLibManager()
	{
		LOG_MESSAGE("* Shut down: Dynamic Library Manager");

		StringDynLibMap::iterator it;

		// unload and delete resources
		for (it = mLibsMap.begin(); it != mLibsMap.end(); it++)
		{
			it->second->unload();
			delete it->second;
		}

		// Empty the list
		mLibsMap.clear();

	}

	DynLibManager* DynLibManager::Instance()
	{
		if (!m_instance)
			m_instance = new DynLibManager();

		return m_instance;
	}

	void DynLibManager::shutDown()
	{
		if (m_instance)
			delete m_instance;

		m_instance = 0;
	}

	void DynLibManager::initialize()
	{
		LOG_MESSAGE("* Initialize: Dynamic Library Manager");

		LOG_MESSAGE("Manager successfully initialized");
	}

	DynLib* DynLibManager::load(const std::string &fileName)
	{
		StringDynLibMap::iterator it = mLibsMap.find(fileName);

		if (it != mLibsMap.end())
			return it->second;
		else
			{
				DynLib *pLib = new DynLib(fileName);
				pLib->load();
				mLibsMap[fileName] = pLib;
				return pLib;
			}

		return 0;
	}

	void DynLibManager::unload(DynLib *library)
	{
		StringDynLibMap::iterator it = mLibsMap.find(library->getName());

		if (it != mLibsMap.end())
			mLibsMap.erase(it);

		library->unload();
		delete library;
	}

}
/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#include "MyGUI_DummyDataManager.h"
#include "MyGUI_DummyDiagnostic.h"
#include "MyGUI_DataFileStream.h"

namespace MyGUI
{

	DummyDataManager::DummyDataManager()
	{
	}

	void DummyDataManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void DummyDataManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IDataStream* DummyDataManager::getData(const std::string& _name) const
	{
		return nullptr;
	}

	void DummyDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool DummyDataManager::isDataExist(const std::string& _name) const
	{
		return false;
	}

	const VectorString& DummyDataManager::getDataListNames(const std::string& _pattern) const
	{
		static VectorString result;
		return result;
	}

	std::string DummyDataManager::getDataPath(const std::string& _name) const
	{
		return {};
	}

} // namespace MyGUI

/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportDataManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_DataFileStream.h"

namespace MyGUI
{

	ExportDataManager::ExportDataManager()
	{
	}

	void ExportDataManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void ExportDataManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IDataStream* ExportDataManager::getData(const std::string& _name)
	{
		return nullptr;
	}

	void ExportDataManager::destroyData(IDataStream* _data)
	{
		delete _data;
	}

	bool ExportDataManager::isDataExist(const std::string& _name)
	{
		return false;
	}

	const VectorString& ExportDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		return result;
	}

	const std::string& ExportDataManager::getDataPath(const std::string& _name)
	{
		static std::string result;
		return result;
	}

} // namespace MyGUI

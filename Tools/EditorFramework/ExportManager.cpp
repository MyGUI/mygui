/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ExportManager.h"
#include "SettingsManager.h"
#include "FactoryManager.h"

template <> tools::ExportManager* MyGUI::Singleton<tools::ExportManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ExportManager>::mClassTypeName = "ExportManager";

namespace tools
{

	ExportManager::ExportManager() :
		mExportSerializer(nullptr)
	{
	}

	ExportManager::~ExportManager()
	{
	}

	void ExportManager::initialise()
	{
		std::string serializer = SettingsManager::getInstance().getValue("Editor/ExportSerializer");
		mExportSerializer = components::FactoryManager::GetInstance().CreateItem<IExportSerializer>(serializer);
	}

	void ExportManager::shutdown()
	{
		delete mExportSerializer;
		mExportSerializer = nullptr;
	}

	void ExportManager::serialization(pugi::xml_document& _doc)
	{
		if (mExportSerializer != nullptr)
			mExportSerializer->serialization(_doc);
	}

	bool ExportManager::deserialization(pugi::xml_document& _doc)
	{
		if (mExportSerializer != nullptr)
			return mExportSerializer->deserialization(_doc);
		return false;
	}

}

/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportDataManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_DataMemoryStream.h"
#include "ExportDataManager.h"

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
		MapData::iterator item = mDatas.find(_name);
		if (item != mDatas.end())
		{
			(*item).second.second ++;
			return (*item).second.first;
		}

		size_t size = 0;
		void* data = nullptr;
		if (Export::ScopeDataManager_GetData::mExportHandle != nullptr)
			size = Export::Convert<size_t>::From(Export::ScopeDataManager_GetData::mExportHandle(Export::Convert<const std::string&>::To(_name), Export::Convert<void*&>::To(data)));

		if (data == nullptr)
			return nullptr;

		MyGUI::DataMemoryStream* stream = new MyGUI::DataMemoryStream(reinterpret_cast<unsigned char*>(data), size);
		mDatas[_name] = DataCounter(stream, 1);

		return stream;
	}

	void ExportDataManager::freeData(IDataStream* _data)
	{
		if (_data == nullptr)
			return;

		std::string name;

		for (MapData::iterator item = mDatas.begin(); item != mDatas.end(); item ++)
		{
			if ((*item).second.first == _data)
			{
				if ((*item).second.second > 1)
				{
					(*item).second.second --;
					return;
				}
				else
				{
					name = (*item).first;
					mDatas.erase(item);
					break;
				}
			}
		}

		delete _data;

		if (Export::ScopeDataManager_FreeData::mExportHandle != nullptr)
			Export::ScopeDataManager_FreeData::mExportHandle(Export::Convert<const std::string&>::To(name));
	}

	bool ExportDataManager::isDataExist(const std::string& _name)
	{
		if (Export::ScopeDataManager_IsDataExist::mExportHandle != nullptr)
			return Export::Convert<bool>::From(Export::ScopeDataManager_IsDataExist::mExportHandle(Export::Convert<const std::string&>::To(_name)));
		return false;
	}

	const VectorString& ExportDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		result.clear();

		size_t count = 0;
		if (Export::ScopeDataManager_GetDataListSize::mExportHandle != nullptr)
			count = Export::Convert<size_t>::From(Export::ScopeDataManager_GetDataListSize::mExportHandle(Export::Convert<const std::string&>::To(_pattern)));

		if (Export::ScopeDataManager_GetDataListItem::mExportHandle != nullptr)
		{
			for (size_t index = 0; index < count; index ++)
				result.push_back(Export::Convert<const std::string&>::From(Export::ScopeDataManager_GetDataListItem::mExportHandle(Export::Convert<size_t>::To(index))));
		}

		if (Export::ScopeDataManager_GetDataListComplete::mExportHandle != nullptr)
			Export::ScopeDataManager_GetDataListComplete::mExportHandle();

		return result;
	}

	const std::string& ExportDataManager::getDataPath(const std::string& _name)
	{
		static std::string result;

		if (Export::ScopeDataManager_GetDataPath::mExportHandle != nullptr)
			result = Export::Convert<const std::string&>::From(Export::ScopeDataManager_GetDataPath::mExportHandle(Export::Convert<const std::string&>::To(_name)));

		return result;
	}

} // namespace MyGUI

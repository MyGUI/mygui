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

} // namespace MyGUI

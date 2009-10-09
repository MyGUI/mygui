/*!
	@file
	@author		George Evmenov
	@date		07/2009
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_OpenGLDataManager.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo/FileSystemInfo.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLDataManager);

	void OpenGLDataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLDataManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* OpenGLDataManager::getData(const std::string& _name)
	{
		std::string file = getDataPath(_name);

		std::ifstream* stream = new std::ifstream();
		stream->open(file.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(stream);

		return data;
	}

	bool OpenGLDataManager::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return files.size() == 1;
	}

	const VectorString& OpenGLDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		result.clear();

		common::VectorWString wresult;
		std::wstring pattern = MyGUI::UString(_pattern).asWStr();

		for (VectorArhivInfo::const_iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, pattern, false);
		}

		for (common::VectorWString::const_iterator item=wresult.begin(); item!=wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		return result;
	}

	void OpenGLDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = MyGUI::UString(_name).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

	std::string OpenGLDataManager::getDataPath(const std::string& _pattern)
	{
		VectorString result;

		common::VectorWString wresult;
		std::wstring pattern = MyGUI::UString(_pattern).asWStr();

		for (VectorArhivInfo::const_iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, pattern, true);
		}

		for (common::VectorWString::const_iterator item=wresult.begin(); item!=wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		if (result.size() == 1) return result[0];
		return "";
	}

} // namespace MyGUI

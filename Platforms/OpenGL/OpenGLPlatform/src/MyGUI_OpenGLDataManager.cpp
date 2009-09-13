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
		return nullptr;
	}

	bool OpenGLDataManager::isDataExist(
		const std::string& _pattern,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, false, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return true;
		return false;
	}

	std::string OpenGLDataManager::getDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, _fullpath, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return files[0];
		return "";
	}

	const VectorString& OpenGLDataManager::getVectorDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _fullmatch)
	{
		static VectorString result;
		result.clear();

		return result;
	}

} // namespace MyGUI

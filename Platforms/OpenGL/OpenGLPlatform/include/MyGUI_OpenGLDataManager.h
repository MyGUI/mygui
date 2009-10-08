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
#ifndef __MYGUI_OPENGL_DATA_MANAGER_H__
#define __MYGUI_OPENGL_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OpenGLDataManager :
		public DataManager
	{
		MYGUI_INSTANCE_HEADER(OpenGLDataManager);

	public:
		void initialise();
		void shutdown();

		virtual IDataStream* getData(const std::string& _name);

		virtual bool isDataExist(const std::string& _name);

		virtual const VectorString& getVectorDataPath(const std::string& _pattern);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);
		std::string getDataPath(const std::string& _name);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_DATA_MANAGER_H__

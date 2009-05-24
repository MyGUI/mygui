/*!
	@file
	@author		Albert Semenov
	@date		05/2008
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
#ifndef __MYGUI_OGRE_DATA_MANAGER_H__
#define __MYGUI_OGRE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OgreDataManager :
		public DataManager
	{
		MYGUI_INSTANCE_HEADER(OgreDataManager);

	public:
		void initialise();
		void shutdown();

		virtual Data* getData(const std::string& _name, const std::string& _group);

		typedef std::vector<std::string> VectorString;

		virtual bool isDataExist(
			const std::string& _pattern,
			const std::string& _group,
			bool _unique,
			bool _fullmatch);

		virtual std::string getDataPath(
			const std::string& _pattern,
			const std::string& _group,
			bool _fullpath,
			bool _unique,
			bool _fullmatch);

		virtual const VectorString& getVectorDataPath(
			const std::string& _pattern,
			const std::string& _group,
			bool _fullpath,
			bool _fullmatch);

	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__

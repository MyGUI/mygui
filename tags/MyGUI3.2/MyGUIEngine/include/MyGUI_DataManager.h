/*!
	@file
	@author		Albert Semenov
	@date		05/2009
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
#ifndef __MYGUI_DATA_MANAGER_H__
#define __MYGUI_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_IDataStream.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT DataManager :
		public Singleton<DataManager>
	{
	public:

		/** Get data stream from specified resource name.
			@param _name Resource name (usually file name).
		*/
		virtual IDataStream* getData(const std::string& _name) = 0;

		/** Is data with specified name exist.
			@param _name Resource name.
		*/
		virtual bool isDataExist(const std::string& _name) = 0;

		/** Get all data names with names that matches pattern.
			@param _pattern Pattern to match (for example "*.layout").
		*/
		virtual const VectorString& getDataListNames(const std::string& _pattern) = 0;

		/** Get full path to data.
			@param _name Resource name.
			@return Return full path to specified data.
			For example getDataPath("My.layout") might return "C:\path\to\project\data\My.layout"
		*/
		virtual const std::string& getDataPath(const std::string& _name) = 0;
	};

} // namespace MyGUI

#endif // __MYGUI_DATA_MANAGER_H__

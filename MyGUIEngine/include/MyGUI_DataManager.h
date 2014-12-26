/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DATA_MANAGER_H_
#define MYGUI_DATA_MANAGER_H_

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

		/** Free data stream.
			@param _data Data stream.
		*/
		virtual void freeData(IDataStream* _data) = 0;

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

#endif // MYGUI_DATA_MANAGER_H_

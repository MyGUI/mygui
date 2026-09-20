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

	class DataStreamHolder;

	class MYGUI_EXPORT DataManager
	{
		MYGUI_SINGLETON_DECLARATION(DataManager);

	public:
		DataManager();
		virtual ~DataManager() = default;

		/** Get data stream from specified resource name.
			@param _name Resource name (usually file name).
		*/
		virtual IDataStream* getData(const std::string& _name) const = 0;

		/** Open a resource and own its stream until the returned holder is destroyed.
			Returns an empty holder if getData() returns nullptr.
		*/
		[[nodiscard]] DataStreamHolder getDataHolder(const std::string& _name) const;

		/** Free data stream.
			@param _data Data stream.
		*/
		virtual void freeData(IDataStream* _data) = 0;

		/** Is data with specified name exist.
			@param _name Resource name.
		*/
		virtual bool isDataExist(const std::string& _name) const = 0;

		/** Get all data names with names that matches pattern.
			Overriding this method is optional, but may be useful in platform implementations.
			@param _pattern Pattern to match (for example "*.layout").
		*/
		virtual VectorString getDataListNames(const std::string& _pattern) const
		{
			return {};
		}

		/** Get full path to data.
			Overriding this method is optional, but may be useful in platform implementations.
			@param _name Resource name.
			@return Return full path to specified data.
			For example getDataPath("My.layout") might return "C:\path\to\project\data\My.layout"
		*/
		virtual std::string getDataPath(const std::string& _name) const
		{
			return {};
		}
	};

} // namespace MyGUI

#endif // MYGUI_DATA_MANAGER_H_

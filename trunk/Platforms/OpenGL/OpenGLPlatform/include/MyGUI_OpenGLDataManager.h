/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
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
		MYGUI_INSTANCE_HEADER(OpenGLDataManager)

	public:
		void initialise();
		void shutdown();

		virtual IDataStream* getData(const std::string& _name);

		virtual bool isDataExist(const std::string& _name);

		virtual const VectorString& getDataListNames(const std::string& _pattern);

		const std::string& getDataPath(const std::string& _name);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

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

/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __EXPORT_MANAGER_H__
#define __EXPORT_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	class ExportManager :
		public MyGUI::Singleton<ExportManager>
	{
	public:
		ExportManager();
		virtual ~ExportManager();

		void initialise();
		void shutdown();

		void convertSkin(MyGUI::xml::Element* _from, MyGUI::xml::Element* _to);

	private:
		MyGUI::UString convertStateName(const MyGUI::UString& _value) const;
	};

} // namespace tools

#endif // __EXPORT_MANAGER_H__

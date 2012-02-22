/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "MyGUI_Plugin.h"

namespace plugin
{

	class Plugin :
		public MyGUI::IPlugin
	{
	public:
		Plugin();
		~Plugin();

	public:
		//!	Initialization
		virtual void initialize();

		//!	Installation
		virtual void install();

		//!	Shut down
		virtual void shutdown();

		//!	Uninstall
		virtual void uninstall();

		//! Get name
		virtual const std::string& getName() const;

	private:
		void update(float _time);

	private:
		static const std::string LogSection;
	};

} // namespace plugin

#endif // __PLUGIN_H__

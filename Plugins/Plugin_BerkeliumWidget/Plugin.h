/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/

#ifndef PLUGIN_H_
#define PLUGIN_H_

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

#endif // PLUGIN_H_

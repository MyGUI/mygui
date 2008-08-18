#ifndef __TEST_PLUGIN_H__
#define __TEST_PLUGIN_H__

#include "MyGUI_Plugin.h"
#include "StrangeButtonFactory.h"

/*!	Test plugin to demostrate possibilities of plugins for MyGUI
*/
class TestPlugin : public MyGUI::Plugin
{

public:
	TestPlugin();
	~TestPlugin();

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
	// фабрики виджетов
	MyGUI::factory::StrangeButtonFactory * mStrangeButtonFactory;

	static const std::string LogSection;

};

#endif // __TEST_PLUGIN_H__
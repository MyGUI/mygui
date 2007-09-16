
#ifndef _TESTPLUGIN_H_
#define _TESTPLUGIN_H_

#include "Plugin.h"

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
	virtual void shutDown();
	
	//!	Uninstall
	virtual void uninstall();

	//! Get name
	virtual MyGUI::String getName() const;
};





#endif
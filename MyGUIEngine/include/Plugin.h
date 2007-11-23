/*!
	@file		Plugin.h
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "Prerequest.h"

namespace MyGUI
{

	/*!	\brief Base plugin class
	*/
	class _MyGUIExport Plugin
	{
	public:
		Plugin() {};

		virtual ~Plugin() {};

		/*!	Get the name of the plugin. 
			@remarks An implementation must be supplied for this method to uniquely
			identify the plugin
		*/
		virtual const std::string& getName() const = 0;

		/*!	Perform the plugin initial installation sequence
		*/
		virtual void install() = 0;

		/*! Perform any tasks the plugin needs to perform on full system
			initialisation.
		*/
		virtual void initialize() = 0;

		/*!	Perform any tasks the plugin needs to perform when the system is shut down
		*/
		virtual void shutdown() = 0;

		/*!	Perform the final plugin uninstallation sequence
		*/
		virtual void uninstall() = 0;
	};

}

#endif // __PLUGIN_H__
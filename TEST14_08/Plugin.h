/*!
	@file		Plugin.h
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "Prerequest.h"

#include <string>

namespace widget
{

	/*!	\brief Base plugin class
	*/
	class Plugin
	{
	public:
		Plugin() {};

		virtual ~Plugin() {};

		/*!	Get the name of the plugin. 
			@remarks An implementation must be supplied for this method to uniquely
			identify the plugin
		*/
		virtual std::string getName() const = 0;

		/*!	Perform the plugin initial installation sequence
		*/
		virtual void install() = 0;

		/*! Perform any tasks the plugin needs to perform on full system
			initialisation.
		*/
		virtual void initialize() = 0;

		/*!	Perform any tasks the plugin needs to perform when the system is shut down
		*/
		virtual void shutDown() = 0;

		/*!	Perform the final plugin uninstallation sequence
		*/
		virtual void uninstall() = 0;
	};

}

#endif
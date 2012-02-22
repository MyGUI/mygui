/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/
#ifndef __HIKARI_WIDGET_H__
#define __HIKARI_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Canvas.h"
#include "FlashControl.h"

namespace Hikari
{

	class MYGUI_EXPORT_DLL HikariWidget :
		public MyGUI::Canvas
	{
		MYGUI_RTTI_DERIVED( HikariWidget );

	public:
		HikariWidget();

		/**
		* Loads a movie (a .swf file) into this FlashControl and begins playing.
		*
		* @param	movieFilename	The filename of the movie to load.
		*
		* @note	The specified movie should reside in the "assetsDirectory" that
		*		was declared when the HikariManager was instantiated.
		*/
		void load(const std::string& movieFilename);

		/**
		* Plays the currently-loaded movie.
		*/
		void play();

		/**
		* Stops the currently-loaded movie.
		*/
		void stop();

		/**
		* Rewinds the currently-loaded movie to the beginning.
		*/
		void rewind();

		/**
		* Navigates the currently-loaded movie to a certain frame and stops there.
		*/
		void gotoFrame(long frameNum);

		/**
		* Sets whether or not the currently-loaded movie should restart when
		* it reaches the end.
		*
		* @param	shouldLoop	Whether the currently-loaded movie should loop.
		*/
		void setLoop(bool shouldLoop);

		/**
		* Sets whether or not the currently-loaded movie should use a
		* transparent background instead of the default background-color.
		*
		* @param	isTransparent	Whether or not the movie should use "transparent" rendering.
		* @param	useAlphaHack	With some Flash versions, there are certain glitches with
		*							transparent rendering (usually with text and aliased geometry).
		*							Set this parameter to 'true' to use an alternative alpha-rendering
		*							hack that may mitigate these issues at the cost of some performance.
		*/
		void setTransparent(bool isTransparent);

		/**
		* Sets the Flash rendering quality for the currently-loaded movie.
		*
		* @param	renderQuality	The RenderQuality to use.
		*/
		void setQuality(short renderQuality);

		/**
		* Sets the scaling mode to use when the aspect ratio of the movie and control do not match.
		*
		* @param	scaleMode	The ScaleMode to use.
		*/
		void setScaleMode(short scaleMode);

		/**
		* Binds a local callback to a certain function name so that your Flash movie can call the function
		* from ActionScript using ExternalInterface.call('functionName').
		*
		* @param	funcName	The name to bind this callback to.
		* @param	callback	The local function to call, see below for examples of declaring a FlashDelegate.
		*
		*	\code
		*	// Example declaration of a compatible function (static function):
		*	FlashValue myStaticFunction(FlashControl* caller, const Arguments& args)
		*	{
		*		// Handle the callback here
		*		return FlashValue();
		*	}
		*
		*	// Example declaration of a compatible function (member function):
		*	FlashValue MyClass::myMemberFunction(FlashControl* caller, const Arguments& args)
		*	{
		*		// Handle the callback here
		*		return "Some return value!";
		*	}
		*
		*	// FlashDelegate (member function) instantiation:
		*	FlashDelegate callback(this, &MyClass::myMemberFunction); // within a class
		*	FlashDelegate callback2(pointerToClassInstance, &MyClass::myMemberFunction);
		*
		*	// FlashDelegate (static function) instantiation:
		*	FlashDelegate callback(&myStaticFunction);
		*	\endcode
		*/
		void bind(const MyGUI::UString& funcName, FlashDelegate::IDelegate* callback);

		/**
		* Un-binds the specified callback.
		*
		* @param	funcName	The name that the callback was bound to.
		*/
		void unbind(const MyGUI::UString& funcName);

		/**
		* Attempts to call a function declared as a callback in the ActionScript of the currently-loaded movie.
		*
		* @param	funcName	The name of the callback that was declared using 'ExternalInterface.addCallback(funcName, function)'
		*						in the ActionScript of the currently-loaded movie.
		* @param	args	The arguments to pass to the ActionScript function.
		*
		* @return	If the invocation was successful and the ActionScript function returned a value, returns a FlashValue with a non-null type.
		*
		* @note	It is highly recommended to use the 'Args(arg1)(arg2)(arg3)...' helper class to pass arguments.
		*/

		FlashValue callFunction(MyGUI::UString funcName, const Arguments& args = Args());

		FlashControl* getControl();

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void notifyUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event);
		void notifyFrameStart(float _time);

		virtual void onMouseDrag(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseMove(int _left, int _top);
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id);

	private:
		FlashControl* mControl;
	};

} // namespace Hikari

#endif // __HIKARI_WIDGET_H__

/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef FlashControl_H_
#define FlashControl_H_

#include <windows.h>
#include "FlashValue.h"
#include <MyGUI.h>

namespace ShockwaveFlashObjects
{
	struct IShockwaveFlash;
}
struct IOleObject;
class IOleInPlaceObjectWindowless;

namespace Hikari
{

	class FlashSite;
	class FlashHandler;
	class RenderBuffer;

	/**
	* Used by FlashControl::setQuality, defines the Flash rendering quality.
	*
	* <ul>
	* <li>RQ_LOW - Favors playback speed over appearance and never uses anti-aliasing.
	* <li>RQ_MEDIUM - Applies some anti-aliasing and does not smooth bitmaps. It produces a better quality than the Low setting, but lower quality than the High setting.
	* <li>RQ_HIGH - Favors appearance over playback speed and always applies anti-aliasing. If the movie does not contain animation, bitmaps are smoothed; if the movie has animation, bitmaps are not smoothed.
	* <li>RQ_BEST - Provides the best display quality and does not consider playback speed. All output is anti-aliased and all bitmaps are smoothed.
	* <li>RQ_AUTOLOW - Emphasizes speed at first but improves appearance whenever possible. Playback begins with anti-aliasing turned off. If the Flash Player detects that the processor can handle it, anti-aliasing is turned on.
	* <li>RQ_AUTOHIGH - Emphasizes playback speed and appearance equally at first but sacrifices appearance for playback speed if necessary. Playback begins with anti-aliasing turned on. If the actual frame rate drops below the specified frame rate, anti-aliasing is turned off to improve playback speed.
	* </ul>
	*/
	enum RenderQuality
	{
		RQ_LOW,
		RQ_MEDIUM,
		RQ_HIGH,
		RQ_BEST,
		RQ_AUTOLOW,
		RQ_AUTOHIGH
	};

	/**
	* Used by FlashControl::setScaleMode, defines the scaling mode to use when the aspect ratio of the control does not match that of the movie.
	*
	* <ul>
	* <li>SM_SHOWALL - Preserves the movie's aspect ratio by adding borders. (Default)
	* <li>SM_NOBORDER - Preserves the movie's aspect ratio by cropping the sides.
	* <li>SM_EXACTFIT - Does not preserve the movie's aspect ratio, scales the movie to the dimensions of the control.
	*/
	enum ScaleMode
	{
		SM_SHOWALL,
		SM_NOBORDER,
		SM_EXACTFIT
	};

	class HikariWidget;
	typedef MyGUI::delegates::CDelegate3<HikariWidget*, const Arguments&, FlashValue&> FlashDelegate;

	/**
	* The FlashControl class is an instance of the Flash Player that is rendered dynamically to a texture and
	* wrapped in an optional movable overlay using the Ogre3D engine.
	*/
	class FlashControl
	{
		friend class FlashSite;
		friend class FlashHandler;

	public:
		FlashControl(HikariWidget* _owner, HMODULE _lib);
		~FlashControl();

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
		* Injects a mouse-move event into this FlashControl (in the control's local coordinate-space).
		*
		* @param	xPos	The local X-coordinate.
		* @param	yPos	The local Y-coordinate.
		*/
		void injectMouseMove(int xPos, int yPos);

		/**
		* Injects a mouse-down event into this FlashControl (in the control's local coordinate-space).
		*
		* @param	xPos	The local X-coordinate.
		* @param	yPos	The local Y-coordinate.
		*/
		void injectMouseDown(int xPos, int yPos);

		/**
		* Injects a mouse-up event into this FlashControl (in the control's local coordinate-space).
		*
		* @param	xPos	The local X-coordinate.
		* @param	yPos	The local Y-coordinate.
		*/
		void injectMouseUp(int xPos, int yPos);

		/**
		* Injects a mouse-wheel event into this FlashControl (in the control's local coordinate-space).
		*
		* @param	relScroll	The relative scroll amount of the mouse-wheel.
		* @param	xPos	The local X-coordinate of the mouse.
		* @param	yPos	The local Y-coordinate of the mouse.
		*/
		void injectMouseWheel(int relScroll, int xPos, int yPos);

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
		*		return FLASH_VOID;
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

		void setSize(int _width, int _height);

		int getWidth() const
		{
			return width;
		}
		int getHeight() const
		{
			return height;
		}

		void invalidateTotally();
		void update();

		ShockwaveFlashObjects::IShockwaveFlash* getFlashInterface()
		{
			return flashInterface;
		}

		void handleKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam);

	protected:
		void createControl(HMODULE _lib);
		void handleFlashCall(const std::wstring& xmlString);

	protected:
		FlashSite* site;
		FlashHandler* handler;
		ShockwaveFlashObjects::IShockwaveFlash* flashInterface;
		IOleObject*	oleObject;
		IOleInPlaceObjectWindowless* windowlessObject;

		int width, height;
		int state;
		int comCount;
		HDC mainContext, altContext;
		HBITMAP mainBitmap, altBitmap;
		BYTE* mainBuffer, *altBuffer;
		RenderBuffer* renderBuffer;
		RECT dirtyBounds;
		bool isClean, isTotallyDirty;
		bool isTransparent;
		bool mIsInitialise;

		HikariWidget* mOwner;

		typedef std::map<MyGUI::UString, FlashDelegate> DelegateMap;
		typedef DelegateMap::iterator DelegateIter;
		DelegateMap delegateMap;
	};

}

#endif

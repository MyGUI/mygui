using System;
using System.Collections.Generic;
using System.Text;

namespace MyGUI
{
	public partial class Widget
	{
		public Widget()
		{
			mNeedToolTip = false;
			mInheritsPick = false;
			mNeedKeyFocus = false;
			mNeedMouseFocus = true;
			mRootMouseFocus = false;
			mRootKeyFocus = false;
		}

		//* Set need tool tip mode flag. Enable this if you need tool tip events for widget 
		public void setNeedToolTip(bool _value)
		{
			mNeedToolTip = _value;
		}

		//* Get need tool tip mode flag 
		public bool getNeedToolTip()
		{
			return mNeedToolTip;
		}

		//* Set mouse pointer for this widget 
		public void setPointer(string _value)
		{
			mPointer = _value;
		}

		//* Get mouse pointer name for this widget 
		public string getPointer()
		{
			return mPointer;
		}

		//* Set need key focus flag 
		public void setNeedKeyFocus(bool _value)
		{
			mNeedKeyFocus = _value;
		}

		// Is need key focus
		//	If disable this widget won't be reacting on keyboard at all.\n
		// Enabled (true) by default.
		public bool getNeedKeyFocus()
		{
			return mNeedKeyFocus;
		}

		//* Set need mouse focus flag 
		public void setNeedMouseFocus(bool _value)
		{
			mNeedMouseFocus = _value;
		}

		// Is need mouse focus
		// If disable this widget won't be reacting on mouse at all.\n
		// Enabled (true) by default.
		public bool getNeedMouseFocus()
		{
			return mNeedMouseFocus;
		}

		// Set inherits mode flag
		// This mode makes all child widgets pickable even if widget don't
		// need mouse focus (was set setNeedKeyFocus(false) ).\n
		// Disabled (false) by default.
		public void setInheritsPick(bool _value)
		{
			mInheritsPick = _value;
		}

		// Get inherits mode flag 
		public bool getInheritsPick()
		{
			return mInheritsPick;
		}

		// Set picking mask for widget 
		public void setMaskPick(string _filename)
		{
			if (string.IsNullOrEmpty(_filename))
				mOwnMaskPickInfo = new MaskPickInfo();
			else if (!mOwnMaskPickInfo.load(_filename))
				//C++ TO C# CONVERTER TODO TASK: There is no direct equivalent in C# to the C++ __LINE__ macro:
				//C++ TO C# CONVERTER TODO TASK: There is no direct equivalent in C# to the C++ __FILE__ macro:
				MyGUI.LogManager.getInstance().log(DefineConstants.MYGUI_LOG_SECTION, MyGUI.LogLevel.level, MyGUI.LogStream() << "mask not load '" << _filename << "'" << MyGUI.LogStream.End(), __FILE__, __LINE__);
		}

		// Set picking mask for widget 
		public void setMaskPick(MaskPickInfo _info)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy assignment (rather than a reference assignment) - this should be verified and a 'CopyFrom' method should be created if it does not yet exist:
			//ORIGINAL LINE: mOwnMaskPickInfo = _info;
			mOwnMaskPickInfo.CopyFrom(_info);
		}

		//C++ TO C# CONVERTER WARNING: 'const' methods are not available in C#:
		//ORIGINAL LINE: bool isMaskPickInside(const types::TPoint<int>& _point, const types::TCoord<int>& _coord) const
		public bool isMaskPickInside(IntPoint _point, IntCoord _coord)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: return mOwnMaskPickInfo.empty() || mOwnMaskPickInfo.pick(_point, _coord);
			return mOwnMaskPickInfo.empty() || mOwnMaskPickInfo.pick(new types.TPoint(_point), new types.TCoord(_coord));
		}

		public bool getRootMouseFocus()
		{
			return mRootMouseFocus;
		}

		public bool getRootKeyFocus()
		{
			return mRootKeyFocus;
		}

		// Event : Widget lost mouse focus.\n
		// signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
		// @param _sender widget that called this event
		// @param _new widget with mouse focus or 0
		public delegate void HandleMouseLostFocus(Widget _sender, Widget _new);
		public event HandleMouseLostFocus eventMouseLostFocus;

		//		* Event : Widget got mouse focus.\n
		//			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
		//			@param _sender widget that called this event
		//			@param _old widget with mouse focus or 0
		//		
		public delegates.CMultiDelegate2<Widget, Widget> eventMouseSetFocus = new delegates.CMultiDelegate2<Widget, Widget>();

		//		* Event : Widget mouse move with captured widget.\n
		//			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		//			@param _sender widget that called this event
		//			@param _left - pointer position
		//			@param _top - pointer position
		//		
		public delegates.CMultiDelegate4<Widget, int, int, MouseButton> eventMouseDrag = new delegates.CMultiDelegate4<Widget, int, int, MouseButton>();

		//		* Event : Mouse move over widget.\n
		//			signature : void method(MyGUI::Widget* _sender, int _left, int _top)\n
		//			@param _sender widget that called this event
		//			@param _left - pointer position
		//			@param _top - pointer position
		//		
		public delegates.CMultiDelegate3<Widget, int, int> eventMouseMove = new delegates.CMultiDelegate3<Widget, int, int>();

		//		* Event : Mouse wheel over widget.\n
		//			signature : void method(MyGUI::Widget* _sender, int _rel)\n
		//			@param _sender widget that called this event
		//			@param _rel relative wheel position
		//		
		public delegates.CMultiDelegate2<Widget, int> eventMouseWheel = new delegates.CMultiDelegate2<Widget, int>();

		//		* Event : Mouse button pressed.\n
		//			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		//			@param _sender widget that called this event
		//			@param _left - pointer position
		//			@param _top - pointer position
		//			@param _id Mouse button id
		//		
		public delegates.CMultiDelegate4<Widget, int, int, MouseButton> eventMouseButtonPressed = new delegates.CMultiDelegate4<Widget, int, int, MouseButton>();

		//		* Event : Mouse button released.\n
		//			signature : void method(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)\n
		//			@param _sender widget that called this event
		//			@param _left - pointer position
		//			@param _top - pointer position
		//			@param _id Mouse button id
		//		
		public delegates.CMultiDelegate4<Widget, int, int, MouseButton> eventMouseButtonReleased = new delegates.CMultiDelegate4<Widget, int, int, MouseButton>();

		//		* Event : Mouse button pressed and released.\n
		//			signature : void method(MyGUI::Widget* _sender)
		//			@param _sender widget that called this event
		//		
		public delegates.CMultiDelegate1<Widget> eventMouseButtonClick = new delegates.CMultiDelegate1<Widget>();

		//		* Event : Mouse button double click.\n
		//			signature : void method(MyGUI::Widget* _sender)
		//			@param _sender widget that called this event
		//		
		public delegates.CMultiDelegate1<Widget> eventMouseButtonDoubleClick = new delegates.CMultiDelegate1<Widget>();

		//		* Event : Widget lost keyboard focus.\n
		//			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _new)\n
		//			@param _sender widget that called this event
		//			@param _new widget with keyboard focus or 0
		//		
		public delegates.CMultiDelegate2<Widget, Widget> eventKeyLostFocus = new delegates.CMultiDelegate2<Widget, Widget>();

		//		* Event : Widget got keyboard focus.\n
		//			signature : void method(MyGUI::Widget* _sender, MyGUI::Widget* _old)\n
		//			@param _sender widget that called this event
		//			@param _old widget with keyboard focus or 0
		//		
		public delegates.CMultiDelegate2<Widget, Widget> eventKeySetFocus = new delegates.CMultiDelegate2<Widget, Widget>();

		//		* Event : Key pressed.\n
		//			signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char)\n
		//			@param _sender widget that called this event
		//			@param _key code
		//			@param _char of pressed symbol (for multilanguage applications)
		//		
		public delegates.CMultiDelegate3<Widget, KeyCode, uint> eventKeyButtonPressed = new delegates.CMultiDelegate3<Widget, KeyCode, uint>();

		//		* Event : Key released.\n
		//			signature : void method(MyGUI::Widget* _sender, MyGUI::KeyCode _key)\n
		//			@param _sender widget that called this event
		//			@param _key code
		//		
		public delegates.CMultiDelegate2<Widget, KeyCode> eventKeyButtonReleased = new delegates.CMultiDelegate2<Widget, KeyCode>();

		//		* Event : Root widget changed mouse focus.\n
		//			info : this event sends only to root widget\n
		//			signature : void method(MyGUI::Widget* _sender, bool _focus);
		//			@param _sender widget that called this event
		//			@param _focus Is widget got mouse focus.
		//		
		public delegates.CMultiDelegate2<Widget, bool> eventRootMouseChangeFocus = new delegates.CMultiDelegate2<Widget, bool>();

		//		* Event : Root widget changed keyboard focus.\n
		//			info : this event sends only to root widget\n
		//			signature : void method(MyGUI::Widget* _sender, bool _focus);
		//			@param _sender widget that called this event
		//			@param _focus Is widget got keyboard focus.
		//		
		public delegates.CMultiDelegate2<Widget, bool> eventRootKeyChangeFocus = new delegates.CMultiDelegate2<Widget, bool>();

		//		* Event : Event about changing tooltip state.\n
		//			signature : void method(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);
		//			@param _sender widget that called this event
		//			@param _info about tooltip
		//		
		public readonly delegates.CMultiDelegate2<Widget, ToolTipInfo> eventToolTip = new delegates.CMultiDelegate2<Widget, ToolTipInfo>();

		//internal:
		public void _riseMouseLostFocus(Widget _new)
		{
			onMouseLostFocus(_new);
			eventMouseLostFocus((Widget)(this), _new);
		}
		public void _riseMouseSetFocus(Widget _old)
		{
			onMouseSetFocus(_old);
			eventMouseSetFocus((Widget)(this), _old);
		}
		public void _riseMouseDrag(int _left, int _top, MouseButton _id)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: onMouseDrag(_left, _top, _id);
			onMouseDrag(_left, _top, new MouseButton(_id));
			eventMouseDrag((Widget)(this), _left, _top, _id);
		}
		public void _riseMouseMove(int _left, int _top)
		{
			onMouseMove(_left, _top);
			eventMouseMove((Widget)(this), _left, _top);
		}
		public void _riseMouseWheel(int _rel)
		{
			onMouseWheel(_rel);
			eventMouseWheel((Widget)(this), _rel);
		}
		public void _riseMouseButtonPressed(int _left, int _top, MouseButton _id)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: onMouseButtonPressed(_left, _top, _id);
			onMouseButtonPressed(_left, _top, new MouseButton(_id));
			eventMouseButtonPressed((Widget)(this), _left, _top, _id);
		}
		public void _riseMouseButtonReleased(int _left, int _top, MouseButton _id)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: onMouseButtonReleased(_left, _top, _id);
			onMouseButtonReleased(_left, _top, new MouseButton(_id));
			eventMouseButtonReleased((Widget)(this), _left, _top, _id);
		}
		public void _riseMouseButtonClick()
		{
			onMouseButtonClick();
			eventMouseButtonClick((Widget)(this));
		}
		public void _riseMouseButtonDoubleClick()
		{
			onMouseButtonDoubleClick();
			eventMouseButtonDoubleClick((Widget)(this));
		}
		public void _riseKeyLostFocus(Widget _new)
		{
			onKeyLostFocus(_new);
			eventKeyLostFocus((Widget)(this), _new);
		}
		public void _riseKeySetFocus(Widget _old)
		{
			onKeySetFocus(_old);
			eventKeySetFocus((Widget)(this), _old);
		}
		public void _riseKeyButtonPressed(KeyCode _key, uint _char)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: onKeyButtonPressed(_key, _char);
			onKeyButtonPressed(new KeyCode(_key), _char);
			eventKeyButtonPressed((Widget)(this), _key, _char);
		}
		public void _riseKeyButtonReleased(KeyCode _key)
		{
			//C++ TO C# CONVERTER WARNING: The following line was determined to be a copy constructor call - this should be verified and a copy constructor should be created if it does not yet exist:
			//ORIGINAL LINE: onKeyButtonReleased(_key);
			onKeyButtonReleased(new KeyCode(_key));
			eventKeyButtonReleased((Widget)(this), _key);
		}
		public void _riseMouseChangeRootFocus(bool _focus)
		{
			onMouseChangeRootFocus(_focus);
			eventRootMouseChangeFocus((Widget)(this), _focus);
		}
		public void _riseKeyChangeRootFocus(bool _focus)
		{
			onKeyChangeRootFocus(_focus);
			eventRootKeyChangeFocus((Widget)(this), _focus);
		}

		public void _setRootMouseFocus(bool _value)
		{
			mRootMouseFocus = _value;
		}
		public void _setRootKeyFocus(bool _value)
		{
			mRootKeyFocus = _value;
		}

		protected virtual void onMouseLostFocus(Widget _new)
		{
		}
		protected virtual void onMouseSetFocus(Widget _old)
		{
		}
		protected virtual void onMouseDrag(int _left, int _top, MouseButton _id)
		{
		}
		protected virtual void onMouseMove(int _left, int _top)
		{
		}
		protected virtual void onMouseWheel(int _rel)
		{
		}
		protected virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id)
		{
		}
		protected virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id)
		{
		}
		protected virtual void onMouseButtonClick()
		{
		}
		protected virtual void onMouseButtonDoubleClick()
		{
		}
		protected virtual void onKeyLostFocus(Widget _new)
		{
		}
		protected virtual void onKeySetFocus(Widget _old)
		{
		}
		protected virtual void onKeyButtonPressed(KeyCode _key, uint _char)
		{
		}
		protected virtual void onKeyButtonReleased(KeyCode _key)
		{
		}
		protected virtual void onMouseChangeRootFocus(bool _focus)
		{
		}
		protected virtual void onKeyChangeRootFocus(bool _focus)
		{
		}

		private string mPointer;
		private MaskPickInfo mOwnMaskPickInfo = new MaskPickInfo();

		private bool mNeedToolTip;
		private bool mInheritsPick;
		private bool mNeedKeyFocus;
		private bool mNeedMouseFocus;

		private bool mRootMouseFocus;
		private bool mRootKeyFocus;
	}
}

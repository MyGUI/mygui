/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public  class PopupMenu :
		MenuControl
    {
        #region PopupMenu

        protected override string GetWidgetType() { return "PopupMenu"; }

        internal static BaseWidget RequestWrapPopupMenu(BaseWidget _parent, IntPtr _widget)
        {
			PopupMenu widget = new PopupMenu();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreatePopupMenu(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			PopupMenu widget = new PopupMenu();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		
    }
}

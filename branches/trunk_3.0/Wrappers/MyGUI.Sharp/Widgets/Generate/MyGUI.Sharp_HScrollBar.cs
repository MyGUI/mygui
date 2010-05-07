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

    public  class HScrollBar : VScrollBar
    {

        #region HScrollBar

        protected override string GetWidgetType() { return "HScroll"; }

        internal static BaseWidget RequestWrapHScrollBar(BaseWidget _parent, IntPtr _widget)
        {
			HScrollBar widget = new HScrollBar();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateHScrollBar(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			HScrollBar widget = new HScrollBar();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   


   


   

		
    }

}

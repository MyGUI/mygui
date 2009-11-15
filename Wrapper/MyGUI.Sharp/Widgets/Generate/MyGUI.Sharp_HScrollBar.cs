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

        public HScrollBar()
            : base()
        {
        }

        internal HScrollBar(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal HScrollBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "HScroll"; }

        internal static BaseWidget RequestWrapHScrollBar(BaseWidget _parent, IntPtr _widget)
        {
            return new HScrollBar(_parent, _widget);
        }

        internal static BaseWidget RequestCreateHScrollBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new HScrollBar(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   


   


   

		
    }

}

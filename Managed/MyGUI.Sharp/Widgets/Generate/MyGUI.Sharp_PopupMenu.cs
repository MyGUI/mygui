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

    public  class PopupMenu : MenuCtrl
    {

        #region PopupMenu

        public PopupMenu()
            : base()
        {
        }

        internal PopupMenu(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal PopupMenu(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "PopupMenu"; }

        internal static BaseWidget RequestWrapPopupMenu(BaseWidget _parent, IntPtr _widget)
        {
            return new PopupMenu(_parent, _widget);
        }

        internal static BaseWidget RequestCreatePopupMenu(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new PopupMenu(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   

		
    }

}

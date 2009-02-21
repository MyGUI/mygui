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

    public class PopupMenu : MenuCtrl
    {

        #region PopupMenu

        public PopupMenu()
            : base()
        {
        }

        public PopupMenu(IntPtr _widget)
            : base(_widget)
        {
        }

        public PopupMenu(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "PopupMenu"; }

		#endregion
	
		
		//InsertPoint
		
    }

}

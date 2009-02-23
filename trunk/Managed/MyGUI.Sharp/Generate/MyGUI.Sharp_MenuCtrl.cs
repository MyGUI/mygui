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

    public class MenuCtrl : Widget
    {

        #region MenuCtrl

        public MenuCtrl()
            : base()
        {
        }

        public MenuCtrl(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public MenuCtrl(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MenuCtrl"; }

		#endregion
	
		
		//InsertPoint
		
    }

}

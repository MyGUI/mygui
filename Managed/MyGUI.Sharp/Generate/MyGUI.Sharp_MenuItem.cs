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

    public class MenuItem : Widget
    {

        #region MenuItem

        public MenuItem()
            : base()
        {
        }

        public MenuItem(IntPtr _widget)
            : base(_widget)
        {
        }

        public MenuItem(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MenuItem"; }

		#endregion
	
		
		//InsertPoint


















		
    }

}

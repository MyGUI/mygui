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

    public class ListBox : Widget
    {

        #region ListBox

        public ListBox()
            : base()
        {
        }

        public ListBox(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public ListBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "List"; }

		#endregion
	
		
		//InsertPoint
		
    }

}

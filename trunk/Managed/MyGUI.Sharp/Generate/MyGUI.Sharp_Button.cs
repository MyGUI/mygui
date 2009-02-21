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

    public class Button : StaticText
    {

        #region Button

        public Button()
            : base()
        {
        }

        public Button(IntPtr _widget)
            : base(_widget)
        {
        }

        public Button(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Button"; }

		#endregion
	
		
		//InsertPoint








		
    }

}

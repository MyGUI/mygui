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

    public  class MenuBar : MenuControl
    {

        #region MenuBar

        protected override string GetWidgetType() { return "MenuBar"; }

        internal static BaseWidget RequestWrapMenuBar(BaseWidget _parent, IntPtr _widget)
        {
			MenuBar widget = new MenuBar();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMenuBar(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MenuBar widget = new MenuBar();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   

		
    }

}

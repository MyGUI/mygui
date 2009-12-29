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

    public  class MenuBar : MenuCtrl
    {

        #region MenuBar

        public MenuBar()
            : base()
        {
        }

        internal MenuBar(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal MenuBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MenuBar"; }

        internal static BaseWidget RequestWrapMenuBar(BaseWidget _parent, IntPtr _widget)
        {
            return new MenuBar(_parent, _widget);
        }

        internal static BaseWidget RequestCreateMenuBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new MenuBar(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   

		
    }

}

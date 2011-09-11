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

    public  class TabItem : Widget
    {

        #region TabItem

        protected override string GetWidgetType() { return "TabItem"; }

        internal static BaseWidget RequestWrapTabItem(BaseWidget _parent, IntPtr _widget)
        {
			TabItem widget = new TabItem();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateTabItem(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			TabItem widget = new TabItem();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Method SetButtonWidth

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_SetButtonWidth_value( IntPtr _native ,
			  int _value );

		public void SetButtonWidth(
			int _value )
		{
			ExportTabItem_SetButtonWidth_value( mNative , 
				 _value );
		}

		#endregion



   


   


   


   

		
    }

}

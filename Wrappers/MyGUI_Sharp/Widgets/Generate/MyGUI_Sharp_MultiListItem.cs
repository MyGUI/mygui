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
    public  class MultiListItem :
		Widget
    {
        #region MultiListItem

        protected override string GetWidgetType() { return "MultiListItem"; }

        internal static BaseWidget RequestWrapMultiListItem(BaseWidget _parent, IntPtr _widget)
        {
			MultiListItem widget = new MultiListItem();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMultiListItem(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MultiListItem widget = new MultiListItem();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetItemWidth

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListItem_SetItemWidth__value(IntPtr _native,
			int _value);

		public void SetItemWidth(
			int _value)
		{
			ExportMultiListItem_SetItemWidth__value(Native,
				_value);
		}

		#endregion
		#region Method SetItemResizingPolicy

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMultiListItem_SetItemResizingPolicy__value(IntPtr _native,
			[MarshalAs(UnmanagedType.I4)] ResizingPolicy _value);

		public void SetItemResizingPolicy(
			ResizingPolicy _value)
		{
			ExportMultiListItem_SetItemResizingPolicy__value(Native,
				_value);
		}

		#endregion
		
    }
}

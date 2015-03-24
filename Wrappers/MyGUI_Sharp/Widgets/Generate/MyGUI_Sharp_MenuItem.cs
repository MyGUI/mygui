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
    public  class MenuItem :
		Widget
    {
        #region MenuItem

        protected override string GetWidgetType() { return "MenuItem"; }

        internal static BaseWidget RequestWrapMenuItem(BaseWidget _parent, IntPtr _widget)
        {
			MenuItem widget = new MenuItem();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMenuItem(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MenuItem widget = new MenuItem();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Method SetItemChildVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemChildVisible__value(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _value);

		public void SetItemChildVisible(
			bool _value)
		{
			ExportMenuItem_SetItemChildVisible__value(Native,
				_value);
		}

		#endregion
		#region Method CreateItemChild

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_CreateItemChild(IntPtr _native);

		public MenuControl CreateItemChild( )
		{
			return (MenuControl)BaseWidget.GetByNative(ExportMenuItem_CreateItemChild(Native));
		}

		#endregion
		#region Method RemoveItem

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_RemoveItem(IntPtr _native);

		public void RemoveItem( )
		{
			ExportMenuItem_RemoveItem(Native);
		}

		#endregion
		#region Property ItemChecked

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportMenuItem_GetItemChecked(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemChecked(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool ItemChecked
		{
			get { return ExportMenuItem_GetItemChecked(Native); }
			set { ExportMenuItem_SetItemChecked(Native, value); }
		}

		#endregion
		#region Property ItemChild

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetItemChild(IntPtr _native);

		public MenuControl ItemChild
		{
			get { return (MenuControl)BaseWidget.GetByNative(ExportMenuItem_GetItemChild(Native)); }
		}

		#endregion
		#region Property MenuCtrlParent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetMenuCtrlParent(IntPtr _native);

		public MenuControl MenuCtrlParent
		{
			get { return (MenuControl)BaseWidget.GetByNative(ExportMenuItem_GetMenuCtrlParent(Native)); }
		}

		#endregion
		#region Property ItemType

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern MenuItemType ExportMenuItem_GetItemType(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemType(IntPtr _widget, [MarshalAs(UnmanagedType.I4)] MenuItemType _value);

		public MenuItemType ItemType
		{
			get { return ExportMenuItem_GetItemType(Native); }
			set { ExportMenuItem_SetItemType(Native, value); }
		}

		#endregion
		#region Property ItemIndex

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuItem_GetItemIndex(IntPtr _native);

		public uint ItemIndex
		{
			get { return ExportMenuItem_GetItemIndex(Native); }
		}

		#endregion
		#region Property ItemId

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetItemId(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemId(IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)] string _value);

		public string ItemId
		{
			get { return Marshal.PtrToStringAnsi(ExportMenuItem_GetItemId(Native)); }
			set { ExportMenuItem_SetItemId(Native, value); }
		}

		#endregion
		#region Property ItemName

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetItemName(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemName(IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)] string _value);

		public string ItemName
		{
			get { return Marshal.PtrToStringUni(ExportMenuItem_GetItemName(Native)); }
			set { ExportMenuItem_SetItemName(Native, value); }
		}

		#endregion
		
    }
}

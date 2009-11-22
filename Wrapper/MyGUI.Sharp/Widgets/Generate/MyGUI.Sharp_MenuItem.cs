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

    public  class MenuItem : Widget
    {

        #region MenuItem

        public MenuItem()
            : base()
        {
        }

        internal MenuItem(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal MenuItem(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "MenuItem"; }

        internal static BaseWidget RequestWrapMenuItem(BaseWidget _parent, IntPtr _widget)
        {
            return new MenuItem(_parent, _widget);
        }

        internal static BaseWidget RequestCreateMenuItem(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new MenuItem(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   


   		#region Method GetItemChild

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuItem_GetItemChild( IntPtr _native );

		public MenuCtrl GetItemChild( )
		{
			return  ExportMenuItem_GetItemChild( mNative )  ;
		}

		#endregion



   		#region Method GetMenuCtrlParent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuItem_GetMenuCtrlParent( IntPtr _native );

		public MenuCtrl GetMenuCtrlParent( )
		{
			return  ExportMenuItem_GetMenuCtrlParent( mNative )  ;
		}

		#endregion



   		#region Method SetItemChildVisible

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemChildVisible_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetItemChildVisible(
			bool _value )
		{
			ExportMenuItem_SetItemChildVisible_value( mNative , 
				 _value );
		}

		#endregion



   		#region Property ItemType

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MenuItemType ExportMenuItem_GetItemType( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemType( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  MenuItemType _value );

		public MenuItemType ItemType
		{
			get { return  ExportMenuItem_GetItemType( mNative )  ; }
			set { ExportMenuItem_SetItemType( mNative,  value ); }
		}

		#endregion



   


   		#region Method CreateItemChild

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern MenuCtrl ExportMenuItem_CreateItemChild( IntPtr _native );

		public MenuCtrl CreateItemChild( )
		{
			return  ExportMenuItem_CreateItemChild( mNative )  ;
		}

		#endregion



   		#region Method GetItemIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportMenuItem_GetItemIndex( IntPtr _native );

		public uint GetItemIndex( )
		{
			return  ExportMenuItem_GetItemIndex( mNative )  ;
		}

		#endregion



   		#region Property ItemId

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetItemId( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemId( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string ItemId
		{
			get { return  Marshal.PtrToStringAnsi(  ExportMenuItem_GetItemId( mNative )  )  ; }
			set { ExportMenuItem_SetItemId( mNative,  value ); }
		}

		#endregion



   		#region Method RemoveItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_RemoveItem( IntPtr _native );

		public void RemoveItem( )
		{
			ExportMenuItem_RemoveItem(  mNative );
		}

		#endregion



   		#region Property ItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportMenuItem_GetItemData( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemData( IntPtr _widget, [MarshalAs(UnmanagedType.IUnknown)] object _value );

		public object ItemData
		{
			get { return ExportMenuItem_GetItemData( mNative ); }
			set { ExportMenuItem_SetItemData( mNative, value ); }
		}

		#endregion



   


   		#region Property ItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMenuItem_GetItemName( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMenuItem_SetItemName( IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public string ItemName
		{
			get { return  Marshal.PtrToStringUni(  ExportMenuItem_GetItemName( mNative )  )  ; }
			set { ExportMenuItem_SetItemName( mNative,  value ); }
		}

		#endregion



   


   


   


   

		
    }

}

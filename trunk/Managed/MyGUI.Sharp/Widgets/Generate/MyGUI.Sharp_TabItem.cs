﻿/*!
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

        public TabItem()
            : base()
        {
        }

        public TabItem(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public TabItem(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "TabItem"; }

		#endregion
	
		
		//InsertPoint

   		#region Method RemoveItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_RemoveItem( IntPtr _native );

		public void RemoveItem( )
		{
			ExportTabItem_RemoveItem(  mNative );
		}

		#endregion



   		#region Method SetItemSelected

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_SetItemSelected( IntPtr _native );

		public void SetItemSelected( )
		{
			ExportTabItem_SetItemSelected(  mNative );
		}

		#endregion



   		#region Property ItemData

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
		private static extern object ExportTabItem_GetItemData( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_SetItemData( IntPtr _widget, [MarshalAs(UnmanagedType.IUnknown)] object _value );

		public object ItemData
		{
			get { return ExportTabItem_GetItemData( mNative ); }
			set { ExportTabItem_SetItemData( mNative, value ); }
		}

		#endregion



   


   		#region Property ItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPWStr)]
		private static extern string ExportTabItem_GetItemName( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_SetItemName( IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public string ItemName
		{
			get { return ExportTabItem_GetItemName( mNative ); }
			set { ExportTabItem_SetItemName( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportTabItem_GetButtonWidth( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportTabItem_SetButtonWidth( IntPtr _widget,   int _value );

		public int ButtonWidth
		{
			get { return ExportTabItem_GetButtonWidth( mNative ); }
			set { ExportTabItem_SetButtonWidth( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}

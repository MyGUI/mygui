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

    public  class ImageBox : Widget
    {

        #region ImageBox

        protected override string GetWidgetType() { return "ImageBox"; }

        internal static BaseWidget RequestWrapImageBox(BaseWidget _parent, IntPtr _widget)
        {
			ImageBox widget = new ImageBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateImageBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ImageBox widget = new ImageBox();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   


   


   


   


   		#region Method SetItemName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemName_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetItemName(
			string _value )
		{
			ExportImageBox_SetItemName_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetItemGroup

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemGroup_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetItemGroup(
			string _value )
		{
			ExportImageBox_SetItemGroup_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetItemResource

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportImageBox_SetItemResource_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public bool SetItemResource(
			string _name )
		{
			return  ExportImageBox_SetItemResource_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method GetItemFrameRate

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportImageBox_GetItemFrameRate_index( IntPtr _native ,
			  uint _index );

		public float GetItemFrameRate(
			uint _index )
		{
			return  ExportImageBox_GetItemFrameRate_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method SetItemFrameRate

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemFrameRate_index_rate( IntPtr _native ,
			  uint _index ,
			  float _rate );

		public void SetItemFrameRate(
			uint _index ,
			float _rate )
		{
			ExportImageBox_SetItemFrameRate_index_rate( mNative , 
				 _index ,
				 _rate );
		}

		#endregion



   		#region Method DeleteAllItemFrames

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteAllItemFrames_index( IntPtr _native ,
			  uint _index );

		public void DeleteAllItemFrames(
			uint _index )
		{
			ExportImageBox_DeleteAllItemFrames_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method DeleteItemFrame

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteItemFrame_index_indexFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame );

		public void DeleteItemFrame(
			uint _index ,
			uint _indexFrame )
		{
			ExportImageBox_DeleteItemFrame_index_indexFrame( mNative , 
				 _index ,
				 _indexFrame );
		}

		#endregion



   		#region Method SetItemFrame

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemFrame_index_indexFrame_item( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			[In] ref IntCoord _item );

		public void SetItemFrame(
			uint _index ,
			uint _indexFrame ,
			IntCoord _item )
		{
			ExportImageBox_SetItemFrame_index_indexFrame_item( mNative , 
				 _index ,
				 _indexFrame ,
				ref _item );
		}

		#endregion



   		#region Method InsertItemFrameDublicate

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			  uint _indexSourceFrame );

		public void InsertItemFrameDublicate(
			uint _index ,
			uint _indexFrame ,
			uint _indexSourceFrame )
		{
			ExportImageBox_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( mNative , 
				 _index ,
				 _indexFrame ,
				 _indexSourceFrame );
		}

		#endregion



   		#region Method AddItemFrameDublicate

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItemFrameDublicate_index_indexSourceFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexSourceFrame );

		public void AddItemFrameDublicate(
			uint _index ,
			uint _indexSourceFrame )
		{
			ExportImageBox_AddItemFrameDublicate_index_indexSourceFrame( mNative , 
				 _index ,
				 _indexSourceFrame );
		}

		#endregion



   		#region Method InsertItemFrame

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItemFrame_index_indexFrame_item( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			[In] ref IntCoord _item );

		public void InsertItemFrame(
			uint _index ,
			uint _indexFrame ,
			IntCoord _item )
		{
			ExportImageBox_InsertItemFrame_index_indexFrame_item( mNative , 
				 _index ,
				 _indexFrame ,
				ref _item );
		}

		#endregion



   		#region Method AddItemFrame

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItemFrame_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void AddItemFrame(
			uint _index ,
			IntCoord _item )
		{
			ExportImageBox_AddItemFrame_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method DeleteAllItems

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteAllItems( IntPtr _native );

		public void DeleteAllItems( )
		{
			ExportImageBox_DeleteAllItems(  mNative );
		}

		#endregion



   		#region Method DeleteItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_DeleteItem_index( IntPtr _native ,
			  uint _index );

		public void DeleteItem(
			uint _index )
		{
			ExportImageBox_DeleteItem_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItem_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void SetItem(
			uint _index ,
			IntCoord _item )
		{
			ExportImageBox_SetItem_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_AddItem_item( IntPtr _native ,
			[In] ref IntCoord _item );

		public void AddItem(
			IntCoord _item )
		{
			ExportImageBox_AddItem_item( mNative , 
				ref _item );
		}

		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_InsertItem_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void InsertItem(
			uint _index ,
			IntCoord _item )
		{
			ExportImageBox_InsertItem_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method ResetItemSelect

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_ResetItemSelect( IntPtr _native );

		public void ResetItemSelect( )
		{
			ExportImageBox_ResetItemSelect(  mNative );
		}

		#endregion



   		#region Property ItemSelect

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetItemSelect( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetItemSelect( IntPtr _widget,   uint _value );

		public uint ItemSelect
		{
			get { return  ExportImageBox_GetItemSelect( mNative )  ; }
			set { ExportImageBox_SetItemSelect( mNative,  value ); }
		}

		#endregion



   		#region Method GetItemCount

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetItemCount( IntPtr _native );

		public uint GetItemCount( )
		{
			return  ExportImageBox_GetItemCount( mNative )  ;
		}

		#endregion



   		#region Property ImageIndex

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportImageBox_GetImageIndex( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageIndex( IntPtr _widget,   uint _value );

		public uint ImageIndex
		{
			get { return  ExportImageBox_GetImageIndex( mNative )  ; }
			set { ExportImageBox_SetImageIndex( mNative,  value ); }
		}

		#endregion



   		#region Method SetImageTile

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageTile_value( IntPtr _native ,
			 ref IntSize _value );

		public void SetImageTile(
			IntSize _value )
		{
			ExportImageBox_SetImageTile_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageCoord_value( IntPtr _native ,
			[In] ref IntCoord _value );

		public void SetImageCoord(
			IntCoord _value )
		{
			ExportImageBox_SetImageCoord_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageRect

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageRect_value( IntPtr _native ,
			[In] ref IntRect _value );

		public void SetImageRect(
			IntRect _value )
		{
			ExportImageBox_SetImageRect_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageTexture

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageTexture_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetImageTexture(
			string _value )
		{
			ExportImageBox_SetImageTexture_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetImageInfo

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportImageBox_SetImageInfo_texture_coord_tile( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _texture ,
			[In] ref IntCoord _coord ,
			 ref IntSize _tile );

		public void SetImageInfo(
			string _texture ,
			IntCoord _coord ,
			IntSize _tile )
		{
			ExportImageBox_SetImageInfo_texture_coord_tile( mNative , 
				 _texture ,
				ref _coord ,
				ref _tile );
		}

		#endregion



   


   


   

		
    }

}

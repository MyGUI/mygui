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

    public  class StaticImage : Widget
    {

        #region StaticImage

        public StaticImage()
            : base()
        {
        }

        internal StaticImage(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal StaticImage(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "StaticImage"; }

        internal static BaseWidget RequestWrapStaticImage(BaseWidget _parent, IntPtr _widget)
        {
            return new StaticImage(_parent, _widget);
        }

        internal static BaseWidget RequestCreateStaticImage(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new StaticImage(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Method SetProperty

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetProperty_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetProperty(
			string _key ,
			string _value )
		{
			ExportStaticImage_SetProperty_key_value( mNative , 
				 _key ,
				 _value );
		}

		#endregion



   


   


   


   


   		#region Method SetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemName_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetItemName(
			string _value )
		{
			ExportStaticImage_SetItemName_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetItemGroup

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemGroup_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetItemGroup(
			string _value )
		{
			ExportStaticImage_SetItemGroup_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetItemResource

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportStaticImage_SetItemResource_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public bool SetItemResource(
			string _name )
		{
			return  ExportStaticImage_SetItemResource_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method SetItemResource

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportStaticImage_SetItemResource_id( IntPtr _native ,
			[In] ref Guid _id );

		public bool SetItemResource(
			Guid _id )
		{
			return  ExportStaticImage_SetItemResource_id( mNative , 
				ref _id )  ;
		}

		#endregion



   		#region Method GetItemFrameRate

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportStaticImage_GetItemFrameRate_index( IntPtr _native ,
			  uint _index );

		public float GetItemFrameRate(
			uint _index )
		{
			return  ExportStaticImage_GetItemFrameRate_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method SetItemFrameRate

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemFrameRate_index_rate( IntPtr _native ,
			  uint _index ,
			  float _rate );

		public void SetItemFrameRate(
			uint _index ,
			float _rate )
		{
			ExportStaticImage_SetItemFrameRate_index_rate( mNative , 
				 _index ,
				 _rate );
		}

		#endregion



   		#region Method DeleteAllItemFrames

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_DeleteAllItemFrames_index( IntPtr _native ,
			  uint _index );

		public void DeleteAllItemFrames(
			uint _index )
		{
			ExportStaticImage_DeleteAllItemFrames_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method DeleteItemFrame

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_DeleteItemFrame_index_indexFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame );

		public void DeleteItemFrame(
			uint _index ,
			uint _indexFrame )
		{
			ExportStaticImage_DeleteItemFrame_index_indexFrame( mNative , 
				 _index ,
				 _indexFrame );
		}

		#endregion



   		#region Method SetItemFrame

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemFrame_index_indexFrame_item( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			[In] ref IntCoord _item );

		public void SetItemFrame(
			uint _index ,
			uint _indexFrame ,
			IntCoord _item )
		{
			ExportStaticImage_SetItemFrame_index_indexFrame_item( mNative , 
				 _index ,
				 _indexFrame ,
				ref _item );
		}

		#endregion



   		#region Method InsertItemFrameDublicate

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			  uint _indexSourceFrame );

		public void InsertItemFrameDublicate(
			uint _index ,
			uint _indexFrame ,
			uint _indexSourceFrame )
		{
			ExportStaticImage_InsertItemFrameDublicate_index_indexFrame_indexSourceFrame( mNative , 
				 _index ,
				 _indexFrame ,
				 _indexSourceFrame );
		}

		#endregion



   		#region Method AddItemFrameDublicate

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_AddItemFrameDublicate_index_indexSourceFrame( IntPtr _native ,
			  uint _index ,
			  uint _indexSourceFrame );

		public void AddItemFrameDublicate(
			uint _index ,
			uint _indexSourceFrame )
		{
			ExportStaticImage_AddItemFrameDublicate_index_indexSourceFrame( mNative , 
				 _index ,
				 _indexSourceFrame );
		}

		#endregion



   		#region Method InsertItemFrame

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_InsertItemFrame_index_indexFrame_item( IntPtr _native ,
			  uint _index ,
			  uint _indexFrame ,
			[In] ref IntCoord _item );

		public void InsertItemFrame(
			uint _index ,
			uint _indexFrame ,
			IntCoord _item )
		{
			ExportStaticImage_InsertItemFrame_index_indexFrame_item( mNative , 
				 _index ,
				 _indexFrame ,
				ref _item );
		}

		#endregion



   		#region Method AddItemFrame

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_AddItemFrame_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void AddItemFrame(
			uint _index ,
			IntCoord _item )
		{
			ExportStaticImage_AddItemFrame_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method DeleteAllItems

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_DeleteAllItems( IntPtr _native );

		public void DeleteAllItems( )
		{
			ExportStaticImage_DeleteAllItems(  mNative );
		}

		#endregion



   		#region Method DeleteItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_DeleteItem_index( IntPtr _native ,
			  uint _index );

		public void DeleteItem(
			uint _index )
		{
			ExportStaticImage_DeleteItem_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method SetItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItem_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void SetItem(
			uint _index ,
			IntCoord _item )
		{
			ExportStaticImage_SetItem_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method AddItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_AddItem_item( IntPtr _native ,
			[In] ref IntCoord _item );

		public void AddItem(
			IntCoord _item )
		{
			ExportStaticImage_AddItem_item( mNative , 
				ref _item );
		}

		#endregion



   		#region Method InsertItem

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_InsertItem_index_item( IntPtr _native ,
			  uint _index ,
			[In] ref IntCoord _item );

		public void InsertItem(
			uint _index ,
			IntCoord _item )
		{
			ExportStaticImage_InsertItem_index_item( mNative , 
				 _index ,
				ref _item );
		}

		#endregion



   		#region Method ResetItemSelect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_ResetItemSelect( IntPtr _native );

		public void ResetItemSelect( )
		{
			ExportStaticImage_ResetItemSelect(  mNative );
		}

		#endregion



   		#region Property ItemSelect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportStaticImage_GetItemSelect( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemSelect( IntPtr _widget,   uint _value );

		public uint ItemSelect
		{
			get { return  ExportStaticImage_GetItemSelect( mNative )  ; }
			set { ExportStaticImage_SetItemSelect( mNative,  value ); }
		}

		#endregion



   		#region Method GetItemCount

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportStaticImage_GetItemCount( IntPtr _native );

		public uint GetItemCount( )
		{
			return  ExportStaticImage_GetItemCount( mNative )  ;
		}

		#endregion



   		#region Property ImageIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportStaticImage_GetImageIndex( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageIndex( IntPtr _widget,   uint _value );

		public uint ImageIndex
		{
			get { return  ExportStaticImage_GetImageIndex( mNative )  ; }
			set { ExportStaticImage_SetImageIndex( mNative,  value ); }
		}

		#endregion



   		#region Method SetImageTile

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageTile_value( IntPtr _native ,
			 ref IntSize _value );

		public void SetImageTile(
			IntSize _value )
		{
			ExportStaticImage_SetImageTile_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageCoord_value( IntPtr _native ,
			[In] ref IntCoord _value );

		public void SetImageCoord(
			IntCoord _value )
		{
			ExportStaticImage_SetImageCoord_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageRect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageRect_value( IntPtr _native ,
			[In] ref IntRect _value );

		public void SetImageRect(
			IntRect _value )
		{
			ExportStaticImage_SetImageRect_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetImageTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageTexture_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetImageTexture(
			string _value )
		{
			ExportStaticImage_SetImageTexture_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetImageInfo

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageInfo_texture_coord_tile( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _texture ,
			[In] ref IntCoord _coord ,
			 ref IntSize _tile );

		public void SetImageInfo(
			string _texture ,
			IntCoord _coord ,
			IntSize _tile )
		{
			ExportStaticImage_SetImageInfo_texture_coord_tile( mNative , 
				 _texture ,
				ref _coord ,
				ref _tile );
		}

		#endregion



   


   


   

		
    }

}

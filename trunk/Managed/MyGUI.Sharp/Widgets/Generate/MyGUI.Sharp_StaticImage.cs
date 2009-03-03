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

    public  class StaticImage : Widget
    {

        #region StaticImage

        public StaticImage()
            : base()
        {
        }

        public StaticImage(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public StaticImage(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "StaticImage"; }

		#endregion
	
		
		//InsertPoint

   


   


   


   


   		#region Method SetItemName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemName_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public void SetItemName(
			string _name )
		{
			ExportStaticImage_SetItemName_name( mNative , 
				 _name );
		}

		#endregion



   		#region Method SetItemGroup

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetItemGroup_group( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _group );

		public void SetItemGroup(
			string _group )
		{
			ExportStaticImage_SetItemGroup_group( mNative , 
				 _group );
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
			return ExportStaticImage_SetItemResource_name( mNative , 
				 _name );
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
			return ExportStaticImage_SetItemResource_id( mNative , 
				ref _id );
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



   


   


   


   


   


   


   


   


   


   


   


   


   


   


   		#region Property ImageIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportStaticImage_GetImageIndex( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageIndex( IntPtr _widget,   uint _value );

		public uint ImageIndex
		{
			get { return ExportStaticImage_GetImageIndex( mNative ); }
			set { ExportStaticImage_SetImageIndex( mNative,  value ); }
		}

		#endregion



   		#region Method SetImageTile

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageTile_tile( IntPtr _native ,
			 ref IntSize _tile );

		public void SetImageTile(
			IntSize _tile )
		{
			ExportStaticImage_SetImageTile_tile( mNative , 
				ref _tile );
		}

		#endregion



   		#region Method SetImageCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageCoord_coord( IntPtr _native ,
			[In] ref IntCoord _coord );

		public void SetImageCoord(
			IntCoord _coord )
		{
			ExportStaticImage_SetImageCoord_coord( mNative , 
				ref _coord );
		}

		#endregion



   


   		#region Method SetImageTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportStaticImage_SetImageTexture_texture( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _texture );

		public void SetImageTexture(
			string _texture )
		{
			ExportStaticImage_SetImageTexture_texture( mNative , 
				 _texture );
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

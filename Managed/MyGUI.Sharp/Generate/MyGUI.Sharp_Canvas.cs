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

    public class Canvas : Widget
    {

        #region Canvas

        public Canvas()
            : base()
        {
        }

        public Canvas(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public Canvas(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Canvas"; }

		#endregion
	
		
		//InsertPoint





   		#region Property TextureManaged

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureManaged( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetTextureManaged( IntPtr _widget,  bool _value );

		public bool TextureManaged
		{
			get { return ExportCanvas_IsTextureManaged( mNative ); }
			set { ExportCanvas_SetTextureManaged( mNative, value ); }
		}

		#endregion



   		#region Method IsTextureCreated

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureCreated( IntPtr _widget );

		public bool IsTextureCreated( )
		{
			return ExportCanvas_IsTextureCreated( mNative );
		}

		#endregion



   		#region Method IsTextureSrcSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureSrcSize( IntPtr _widget );

		public bool IsTextureSrcSize( )
		{
			return ExportCanvas_IsTextureSrcSize( mNative );
		}

		#endregion





   		#region Method SetCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetCoord_left_top_width_height( IntPtr _widget ,
			 int _left ,
			 int _top ,
			 int _width ,
			 int _height );

		public void SetCoord(
			int _left ,
			int _top ,
			int _width ,
			int _height )
		{
			ExportCanvas_SetCoord_left_top_width_height( mNative , 
				_left ,
				_top ,
				_width ,
				_height );
		}

		#endregion



   		#region Method SetSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetSize_width_height( IntPtr _widget ,
			 int _width ,
			 int _height );

		public void SetSize(
			int _width ,
			int _height )
		{
			ExportCanvas_SetSize_width_height( mNative , 
				_width ,
				_height );
		}

		#endregion



   		#region Method SetCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetCoord_coord( IntPtr _widget ,
			 IntCoord _coord );

		public void SetCoord(
			IntCoord _coord )
		{
			ExportCanvas_SetCoord_coord( mNative , 
				_coord );
		}

		#endregion



   		#region Method SetSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetSize_size( IntPtr _widget ,
			 IntSize _size );

		public void SetSize(
			IntSize _size )
		{
			ExportCanvas_SetSize_size( mNative , 
				_size );
		}

		#endregion



   		#region Method GetTextureGroup

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportCanvas_GetTextureGroup( IntPtr _widget );

		public string GetTextureGroup( )
		{
			return ExportCanvas_GetTextureGroup( mNative );
		}

		#endregion



   		#region Method GetTextureName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportCanvas_GetTextureName( IntPtr _widget );

		public string GetTextureName( )
		{
			return ExportCanvas_GetTextureName( mNative );
		}

		#endregion



   		#region Method GetTextureSrcSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntSize ExportCanvas_GetTextureSrcSize( IntPtr _widget );

		public IntSize GetTextureSrcSize( )
		{
			return ExportCanvas_GetTextureSrcSize( mNative );
		}

		#endregion



   		#region Method GetTextureSrcHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureSrcHeight( IntPtr _widget );

		public int GetTextureSrcHeight( )
		{
			return ExportCanvas_GetTextureSrcHeight( mNative );
		}

		#endregion



   		#region Method GetTextureSrcWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureSrcWidth( IntPtr _widget );

		public int GetTextureSrcWidth( )
		{
			return ExportCanvas_GetTextureSrcWidth( mNative );
		}

		#endregion



   		#region Method GetTextureRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntSize ExportCanvas_GetTextureRealSize( IntPtr _widget );

		public IntSize GetTextureRealSize( )
		{
			return ExportCanvas_GetTextureRealSize( mNative );
		}

		#endregion



   		#region Method GetTextureRealHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureRealHeight( IntPtr _widget );

		public int GetTextureRealHeight( )
		{
			return ExportCanvas_GetTextureRealHeight( mNative );
		}

		#endregion



   		#region Method GetTextureRealWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureRealWidth( IntPtr _widget );

		public int GetTextureRealWidth( )
		{
			return ExportCanvas_GetTextureRealWidth( mNative );
		}

		#endregion



   		#region Method IsLocked

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsLocked( IntPtr _widget );

		public bool IsLocked( )
		{
			return ExportCanvas_IsLocked( mNative );
		}

		#endregion



   		#region Method Unlock

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_Unlock( IntPtr _widget );

		public void Unlock( )
		{
			ExportCanvas_Unlock(  mNative );
		}

		#endregion



   		#region Method UpdateTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_UpdateTexture( IntPtr _widget );

		public void UpdateTexture( )
		{
			ExportCanvas_UpdateTexture(  mNative );
		}

		#endregion



   		#region Method DestroyTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_DestroyTexture( IntPtr _widget );

		public void DestroyTexture( )
		{
			ExportCanvas_DestroyTexture(  mNative );
		}

		#endregion














		
    }

}

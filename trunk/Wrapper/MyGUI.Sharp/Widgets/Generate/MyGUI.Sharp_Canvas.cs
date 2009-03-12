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

    public  class Canvas : Widget
    {

        #region Canvas

        public Canvas()
            : base()
        {
        }

        internal Canvas(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal Canvas(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Canvas"; }

        internal static BaseWidget RequestWrapCanvas(BaseWidget _parent, IntPtr _widget)
        {
            return new Canvas(_parent, _widget);
        }

        internal static BaseWidget RequestCreateCanvas(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new Canvas(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint





   		#region Property TextureManaged

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureManaged( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_SetTextureManaged( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool TextureManaged
		{
			get { return  ExportCanvas_IsTextureManaged( mNative )  ; }
			set { ExportCanvas_SetTextureManaged( mNative,  value ); }
		}

		#endregion



   		#region Method IsTextureCreated

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureCreated( IntPtr _native );

		public bool IsTextureCreated( )
		{
			return  ExportCanvas_IsTextureCreated( mNative )  ;
		}

		#endregion



   		#region Method IsTextureSrcSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsTextureSrcSize( IntPtr _native );

		public bool IsTextureSrcSize( )
		{
			return  ExportCanvas_IsTextureSrcSize( mNative )  ;
		}

		#endregion





   


   


   


   


   		#region Method GetTextureGroup

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportCanvas_GetTextureGroup( IntPtr _native );

		public string GetTextureGroup( )
		{
			return  Marshal.PtrToStringAnsi(  ExportCanvas_GetTextureGroup( mNative )  )  ;
		}

		#endregion



   		#region Method GetTextureName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportCanvas_GetTextureName( IntPtr _native );

		public string GetTextureName( )
		{
			return  Marshal.PtrToStringAnsi(  ExportCanvas_GetTextureName( mNative )  )  ;
		}

		#endregion



   


   		#region Method GetTextureSrcSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportCanvas_GetTextureSrcSize( IntPtr _native );

		public IntSize GetTextureSrcSize( )
		{
			return  (IntSize)Marshal.PtrToStructure(  ExportCanvas_GetTextureSrcSize( mNative )  , typeof(IntSize) )  ;
		}

		#endregion



   		#region Method GetTextureSrcHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureSrcHeight( IntPtr _native );

		public int GetTextureSrcHeight( )
		{
			return  ExportCanvas_GetTextureSrcHeight( mNative )  ;
		}

		#endregion



   		#region Method GetTextureSrcWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureSrcWidth( IntPtr _native );

		public int GetTextureSrcWidth( )
		{
			return  ExportCanvas_GetTextureSrcWidth( mNative )  ;
		}

		#endregion



   		#region Method GetTextureRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportCanvas_GetTextureRealSize( IntPtr _native );

		public IntSize GetTextureRealSize( )
		{
			return  (IntSize)Marshal.PtrToStructure(  ExportCanvas_GetTextureRealSize( mNative )  , typeof(IntSize) )  ;
		}

		#endregion



   		#region Method GetTextureRealHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureRealHeight( IntPtr _native );

		public int GetTextureRealHeight( )
		{
			return  ExportCanvas_GetTextureRealHeight( mNative )  ;
		}

		#endregion



   		#region Method GetTextureRealWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportCanvas_GetTextureRealWidth( IntPtr _native );

		public int GetTextureRealWidth( )
		{
			return  ExportCanvas_GetTextureRealWidth( mNative )  ;
		}

		#endregion



   


   


   


   


   


   		#region Method IsLocked

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportCanvas_IsLocked( IntPtr _native );

		public bool IsLocked( )
		{
			return  ExportCanvas_IsLocked( mNative )  ;
		}

		#endregion



   		#region Method Unlock

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_Unlock( IntPtr _native );

		public void Unlock( )
		{
			ExportCanvas_Unlock(  mNative );
		}

		#endregion



   


   		#region Method UpdateTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_UpdateTexture( IntPtr _native );

		public void UpdateTexture( )
		{
			ExportCanvas_UpdateTexture(  mNative );
		}

		#endregion



   


   		#region Method DestroyTexture

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportCanvas_DestroyTexture( IntPtr _native );

		public void DestroyTexture( )
		{
			ExportCanvas_DestroyTexture(  mNative );
		}

		#endregion









   


   

		
    }

}

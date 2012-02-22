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

    public  class Button : TextBox
    {

        #region Button

        protected override string GetWidgetType() { return "Button"; }

        internal static BaseWidget RequestWrapButton(BaseWidget _parent, IntPtr _widget)
        {
			Button widget = new Button();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateButton(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			Button widget = new Button();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Method SetImageName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageName_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public void SetImageName(
			string _name )
		{
			ExportButton_SetImageName_name( mNative , 
				 _name );
		}

		#endregion



   		#region Method SetImageGroup

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageGroup_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public void SetImageGroup(
			string _name )
		{
			ExportButton_SetImageGroup_name( mNative , 
				 _name );
		}

		#endregion



   		#region Method SetImageResource

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageResource_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public void SetImageResource(
			string _name )
		{
			ExportButton_SetImageResource_name( mNative , 
				 _name );
		}

		#endregion



   		#region Property ModeImage

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportButton_GetModeImage( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetModeImage( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ModeImage
		{
			get { return  ExportButton_GetModeImage( mNative )  ; }
			set { ExportButton_SetModeImage( mNative,  value ); }
		}

		#endregion



   		#region Property StateSelected

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportButton_GetStateSelected( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetStateSelected( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool StateSelected
		{
			get { return  ExportButton_GetStateSelected( mNative )  ; }
			set { ExportButton_SetStateSelected( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}

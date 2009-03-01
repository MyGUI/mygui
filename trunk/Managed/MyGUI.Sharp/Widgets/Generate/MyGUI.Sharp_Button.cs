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

    public  class Button : StaticText
    {

        #region Button

        public Button()
            : base()
        {
        }

        public Button(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public Button(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Button"; }

		#endregion
	
		
		//InsertPoint

   		#region Method GetStaticImage

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern StaticImage ExportButton_GetStaticImage( IntPtr _native );

		public StaticImage GetStaticImage( )
		{
			return ExportButton_GetStaticImage( mNative );
		}

		#endregion



   		#region Property ImageIndex

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportButton_GetImageIndex( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetImageIndex( IntPtr _widget,   uint _value );

		public uint ImageIndex
		{
			get { return ExportButton_GetImageIndex( mNative ); }
			set { ExportButton_SetImageIndex( mNative,  value ); }
		}

		#endregion



   		#region Property StateCheck

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportButton_GetStateCheck( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetStateCheck( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool StateCheck
		{
			get { return ExportButton_GetStateCheck( mNative ); }
			set { ExportButton_SetStateCheck( mNative,  value ); }
		}

		#endregion



   		#region Property ButtonPressed

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportButton_GetButtonPressed( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportButton_SetButtonPressed( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool ButtonPressed
		{
			get { return ExportButton_GetButtonPressed( mNative ); }
			set { ExportButton_SetButtonPressed( mNative,  value ); }
		}

		#endregion


		
    }

}

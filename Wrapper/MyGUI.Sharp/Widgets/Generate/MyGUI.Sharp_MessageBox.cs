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

    public  class MessageBox : Window
    {

        #region MessageBox

        public MessageBox()
            : base()
        {
        }

        internal MessageBox(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal MessageBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Message"; }

        internal static BaseWidget RequestWrapMessageBox(BaseWidget _parent, IntPtr _widget)
        {
            return new MessageBox(_parent, _widget);
        }

        internal static BaseWidget RequestCreateMessageBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new MessageBox(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event MessageBoxResult

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBoxEvent_AdviseMessageBoxResult( IntPtr _native, bool _advise );

		public delegate void HandleMessageBoxResult(
			 MessageBox _sender ,
			 MessageBoxStyle _result );
			
		private HandleMessageBoxResult mEventMessageBoxResult;
		public event HandleMessageBoxResult EventMessageBoxResult
		{
			add
			{
				if (mEventMessageBoxResult == null) ExportMessageBoxEvent_AdviseMessageBoxResult( mNative, true );
				mEventMessageBoxResult += value;
			}
			remove
			{
				mEventMessageBoxResult -= value;
				if (mEventMessageBoxResult == null) ExportMessageBoxEvent_AdviseMessageBoxResult( mNative, false );
			}
		}


		private struct ExportEventMessageBoxResult
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportMessageBoxEvent_DelegateMessageBoxResult( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  MessageBox _sender ,
				[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _result );
				
			private static ExportHandle mDelegate;
			public ExportEventMessageBoxResult( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportMessageBoxEvent_DelegateMessageBoxResult( mDelegate );
			}
		}
		static ExportEventMessageBoxResult mExportMessageBoxResult =
			new ExportEventMessageBoxResult(new ExportEventMessageBoxResult.ExportHandle( OnExportMessageBoxResult ));

		private static void OnExportMessageBoxResult(
			 MessageBox _sender ,
			 MessageBoxStyle _result )
		{
			if (_sender.mEventMessageBoxResult != null)
				_sender.mEventMessageBoxResult(
					 _sender ,
					 _result );
		}

		#endregion



   		#region Method SetMessageStyle

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageStyle_style( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _style );

		public void SetMessageStyle(
			MessageBoxStyle _style )
		{
			ExportMessageBox_SetMessageStyle_style( mNative , 
				 _style );
		}

		#endregion



   		#region Method SetMessageButton

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageButton_button( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _button );

		public void SetMessageButton(
			MessageBoxStyle _button )
		{
			ExportMessageBox_SetMessageButton_button( mNative , 
				 _button );
		}

		#endregion



   		#region Method EndMessage

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_EndMessage( IntPtr _native );

		public void EndMessage( )
		{
			ExportMessageBox_EndMessage(  mNative );
		}

		#endregion



   		#region Method EndMessage

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_EndMessage_result( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _result );

		public void EndMessage(
			MessageBoxStyle _result )
		{
			ExportMessageBox_EndMessage_result( mNative , 
				 _result );
		}

		#endregion



   		#region Method SetWindowFade

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetWindowFade_fade( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _fade );

		public void SetWindowFade(
			bool _fade )
		{
			ExportMessageBox_SetWindowFade_fade( mNative , 
				 _fade );
		}

		#endregion



   		#region Method SetMessageIcon

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageIcon_icon( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _icon );

		public void SetMessageIcon(
			MessageBoxStyle _icon )
		{
			ExportMessageBox_SetMessageIcon_icon( mNative , 
				 _icon );
		}

		#endregion



   		#region Method GetDefaultLayer

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportMessageBox_GetDefaultLayer( IntPtr _native );

		public string GetDefaultLayer( )
		{
			return  Marshal.PtrToStringAnsi(  ExportMessageBox_GetDefaultLayer( mNative )  )  ;
		}

		#endregion



   		#region Method SetSmoothShow

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetSmoothShow_smooth( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _smooth );

		public void SetSmoothShow(
			bool _smooth )
		{
			ExportMessageBox_SetSmoothShow_smooth( mNative , 
				 _smooth );
		}

		#endregion



   		#region Method AddButtonName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern MessageBoxStyle ExportMessageBox_AddButtonName_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _name );

		public MessageBoxStyle AddButtonName(
			string _name )
		{
			return  ExportMessageBox_AddButtonName_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method SetMessageText

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageText_message( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _message );

		public void SetMessageText(
			string _message )
		{
			ExportMessageBox_SetMessageText_message( mNative , 
				 _message );
		}

		#endregion



   


   

		
    }

}

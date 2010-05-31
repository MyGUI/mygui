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

        protected override string GetWidgetType() { return "Message"; }

        internal static BaseWidget RequestWrapMessageBox(BaseWidget _parent, IntPtr _widget)
        {
			MessageBox widget = new MessageBox();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateMessageBox(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			MessageBox widget = new MessageBox();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
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



   


   		#region Method SetMessageModal

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageModal_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetMessageModal(
			bool _value )
		{
			ExportMessageBox_SetMessageModal_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetMessageStyle

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageStyle_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _value );

		public void SetMessageStyle(
			MessageBoxStyle _value )
		{
			ExportMessageBox_SetMessageStyle_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetMessageButton

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageButton_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _value );

		public void SetMessageButton(
			MessageBoxStyle _value )
		{
			ExportMessageBox_SetMessageButton_value( mNative , 
				 _value );
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
		private static extern void ExportMessageBox_SetWindowFade_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetWindowFade(
			bool _value )
		{
			ExportMessageBox_SetWindowFade_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetMessageIcon

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportMessageBox_SetMessageIcon_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  MessageBoxStyle _value );

		public void SetMessageIcon(
			MessageBoxStyle _value )
		{
			ExportMessageBox_SetMessageIcon_value( mNative , 
				 _value );
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
		private static extern void ExportMessageBox_SetSmoothShow_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetSmoothShow(
			bool _value )
		{
			ExportMessageBox_SetSmoothShow_value( mNative , 
				 _value );
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
		private static extern void ExportMessageBox_SetMessageText_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public void SetMessageText(
			string _value )
		{
			ExportMessageBox_SetMessageText_value( mNative , 
				 _value );
		}

		#endregion



   


   


   

		
    }

}

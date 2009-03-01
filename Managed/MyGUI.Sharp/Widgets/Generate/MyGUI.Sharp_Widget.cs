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

    public partial class Widget : BaseWidget
    {

        #region Widget

        public Widget()
            : base()
        {
        }

        public Widget(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        public Widget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Widget"; }

		#endregion
	
		
		//InsertPoint



   		#region Event ActionInfo

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseActionInfo( IntPtr _native, bool _advise );

		public delegate void HandleActionInfo(
			 Widget _sender ,
			 string _key ,
			 string _value );
			
		private HandleActionInfo mEventActionInfo;
		public event HandleActionInfo EventActionInfo
		{
			add
			{
				if (mEventActionInfo == null) ExportWidgetEvent_AdviseActionInfo( mNative, true );
				mEventActionInfo += value;
			}
			remove
			{
				mEventActionInfo -= value;
				if (mEventActionInfo == null) ExportWidgetEvent_AdviseActionInfo( mNative, false );
			}
		}


		private struct ExportEventActionInfo
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateActionInfo( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.LPStr)]  string _key ,
				[MarshalAs(UnmanagedType.LPStr)]  string _value );
				
			private static ExportHandle mDelegate;
			public ExportEventActionInfo( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateActionInfo( mDelegate );
			}
		}
		static ExportEventActionInfo mExportActionInfo =
			new ExportEventActionInfo(new ExportEventActionInfo.ExportHandle( OnExportActionInfo ));

		private static void OnExportActionInfo(
			 Widget _sender ,
			 string _key ,
			 string _value )
		{
			if (_sender.mEventActionInfo != null)
				_sender.mEventActionInfo(
					 _sender ,
					 _key ,
					 _value );
		}

		#endregion



   		#region Event ToolTip

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseToolTip( IntPtr _native, bool _advise );

		public delegate void HandleToolTip(
			 Widget _sender ,
			ref ToolTipInfo _info );
			
		private HandleToolTip mEventToolTip;
		public event HandleToolTip EventToolTip
		{
			add
			{
				if (mEventToolTip == null) ExportWidgetEvent_AdviseToolTip( mNative, true );
				mEventToolTip += value;
			}
			remove
			{
				mEventToolTip -= value;
				if (mEventToolTip == null) ExportWidgetEvent_AdviseToolTip( mNative, false );
			}
		}


		private struct ExportEventToolTip
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateToolTip( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[In] ref ToolTipInfo _info );
				
			private static ExportHandle mDelegate;
			public ExportEventToolTip( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateToolTip( mDelegate );
			}
		}
		static ExportEventToolTip mExportToolTip =
			new ExportEventToolTip(new ExportEventToolTip.ExportHandle( OnExportToolTip ));

		private static void OnExportToolTip(
			 Widget _sender ,
			ref ToolTipInfo _info )
		{
			if (_sender.mEventToolTip != null)
				_sender.mEventToolTip(
					 _sender ,
					ref _info );
		}

		#endregion



   		#region Event RootKeyChangeFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseRootKeyChangeFocus( IntPtr _native, bool _advise );

		public delegate void HandleRootKeyChangeFocus(
			 Widget _sender ,
			 bool _focus );
			
		private HandleRootKeyChangeFocus mEventRootKeyChangeFocus;
		public event HandleRootKeyChangeFocus EventRootKeyChangeFocus
		{
			add
			{
				if (mEventRootKeyChangeFocus == null) ExportWidgetEvent_AdviseRootKeyChangeFocus( mNative, true );
				mEventRootKeyChangeFocus += value;
			}
			remove
			{
				mEventRootKeyChangeFocus -= value;
				if (mEventRootKeyChangeFocus == null) ExportWidgetEvent_AdviseRootKeyChangeFocus( mNative, false );
			}
		}


		private struct ExportEventRootKeyChangeFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateRootKeyChangeFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.U1)]  bool _focus );
				
			private static ExportHandle mDelegate;
			public ExportEventRootKeyChangeFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateRootKeyChangeFocus( mDelegate );
			}
		}
		static ExportEventRootKeyChangeFocus mExportRootKeyChangeFocus =
			new ExportEventRootKeyChangeFocus(new ExportEventRootKeyChangeFocus.ExportHandle( OnExportRootKeyChangeFocus ));

		private static void OnExportRootKeyChangeFocus(
			 Widget _sender ,
			 bool _focus )
		{
			if (_sender.mEventRootKeyChangeFocus != null)
				_sender.mEventRootKeyChangeFocus(
					 _sender ,
					 _focus );
		}

		#endregion



   		#region Event RootMouseChangeFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseRootMouseChangeFocus( IntPtr _native, bool _advise );

		public delegate void HandleRootMouseChangeFocus(
			 Widget _sender ,
			 bool _focus );
			
		private HandleRootMouseChangeFocus mEventRootMouseChangeFocus;
		public event HandleRootMouseChangeFocus EventRootMouseChangeFocus
		{
			add
			{
				if (mEventRootMouseChangeFocus == null) ExportWidgetEvent_AdviseRootMouseChangeFocus( mNative, true );
				mEventRootMouseChangeFocus += value;
			}
			remove
			{
				mEventRootMouseChangeFocus -= value;
				if (mEventRootMouseChangeFocus == null) ExportWidgetEvent_AdviseRootMouseChangeFocus( mNative, false );
			}
		}


		private struct ExportEventRootMouseChangeFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateRootMouseChangeFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.U1)]  bool _focus );
				
			private static ExportHandle mDelegate;
			public ExportEventRootMouseChangeFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateRootMouseChangeFocus( mDelegate );
			}
		}
		static ExportEventRootMouseChangeFocus mExportRootMouseChangeFocus =
			new ExportEventRootMouseChangeFocus(new ExportEventRootMouseChangeFocus.ExportHandle( OnExportRootMouseChangeFocus ));

		private static void OnExportRootMouseChangeFocus(
			 Widget _sender ,
			 bool _focus )
		{
			if (_sender.mEventRootMouseChangeFocus != null)
				_sender.mEventRootMouseChangeFocus(
					 _sender ,
					 _focus );
		}

		#endregion



   		#region Event KeyButtonReleased

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeyButtonReleased( IntPtr _native, bool _advise );

		public delegate void HandleKeyButtonReleased(
			 Widget _sender ,
			 KeyCode _key );
			
		private HandleKeyButtonReleased mEventKeyButtonReleased;
		public event HandleKeyButtonReleased EventKeyButtonReleased
		{
			add
			{
				if (mEventKeyButtonReleased == null) ExportWidgetEvent_AdviseKeyButtonReleased( mNative, true );
				mEventKeyButtonReleased += value;
			}
			remove
			{
				mEventKeyButtonReleased -= value;
				if (mEventKeyButtonReleased == null) ExportWidgetEvent_AdviseKeyButtonReleased( mNative, false );
			}
		}


		private struct ExportEventKeyButtonReleased
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateKeyButtonReleased( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.I4)]  KeyCode _key );
				
			private static ExportHandle mDelegate;
			public ExportEventKeyButtonReleased( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateKeyButtonReleased( mDelegate );
			}
		}
		static ExportEventKeyButtonReleased mExportKeyButtonReleased =
			new ExportEventKeyButtonReleased(new ExportEventKeyButtonReleased.ExportHandle( OnExportKeyButtonReleased ));

		private static void OnExportKeyButtonReleased(
			 Widget _sender ,
			 KeyCode _key )
		{
			if (_sender.mEventKeyButtonReleased != null)
				_sender.mEventKeyButtonReleased(
					 _sender ,
					 _key );
		}

		#endregion



   		#region Event KeyButtonPressed

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeyButtonPressed( IntPtr _native, bool _advise );

		public delegate void HandleKeyButtonPressed(
			 Widget _sender ,
			 KeyCode _key ,
			 uint _char );
			
		private HandleKeyButtonPressed mEventKeyButtonPressed;
		public event HandleKeyButtonPressed EventKeyButtonPressed
		{
			add
			{
				if (mEventKeyButtonPressed == null) ExportWidgetEvent_AdviseKeyButtonPressed( mNative, true );
				mEventKeyButtonPressed += value;
			}
			remove
			{
				mEventKeyButtonPressed -= value;
				if (mEventKeyButtonPressed == null) ExportWidgetEvent_AdviseKeyButtonPressed( mNative, false );
			}
		}


		private struct ExportEventKeyButtonPressed
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateKeyButtonPressed( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.I4)]  KeyCode _key ,
				  uint _char );
				
			private static ExportHandle mDelegate;
			public ExportEventKeyButtonPressed( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateKeyButtonPressed( mDelegate );
			}
		}
		static ExportEventKeyButtonPressed mExportKeyButtonPressed =
			new ExportEventKeyButtonPressed(new ExportEventKeyButtonPressed.ExportHandle( OnExportKeyButtonPressed ));

		private static void OnExportKeyButtonPressed(
			 Widget _sender ,
			 KeyCode _key ,
			 uint _char )
		{
			if (_sender.mEventKeyButtonPressed != null)
				_sender.mEventKeyButtonPressed(
					 _sender ,
					 _key ,
					 _char );
		}

		#endregion



   		#region Event KeySetFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeySetFocus( IntPtr _native, bool _advise );

		public delegate void HandleKeySetFocus(
			 Widget _sender ,
			 Widget _old );
			
		private HandleKeySetFocus mEventKeySetFocus;
		public event HandleKeySetFocus EventKeySetFocus
		{
			add
			{
				if (mEventKeySetFocus == null) ExportWidgetEvent_AdviseKeySetFocus( mNative, true );
				mEventKeySetFocus += value;
			}
			remove
			{
				mEventKeySetFocus -= value;
				if (mEventKeySetFocus == null) ExportWidgetEvent_AdviseKeySetFocus( mNative, false );
			}
		}


		private struct ExportEventKeySetFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateKeySetFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.Interface)]  Widget _old );
				
			private static ExportHandle mDelegate;
			public ExportEventKeySetFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateKeySetFocus( mDelegate );
			}
		}
		static ExportEventKeySetFocus mExportKeySetFocus =
			new ExportEventKeySetFocus(new ExportEventKeySetFocus.ExportHandle( OnExportKeySetFocus ));

		private static void OnExportKeySetFocus(
			 Widget _sender ,
			 Widget _old )
		{
			if (_sender.mEventKeySetFocus != null)
				_sender.mEventKeySetFocus(
					 _sender ,
					 _old );
		}

		#endregion



   		#region Event KeyLostFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeyLostFocus( IntPtr _native, bool _advise );

		public delegate void HandleKeyLostFocus(
			 Widget _sender ,
			 Widget _new );
			
		private HandleKeyLostFocus mEventKeyLostFocus;
		public event HandleKeyLostFocus EventKeyLostFocus
		{
			add
			{
				if (mEventKeyLostFocus == null) ExportWidgetEvent_AdviseKeyLostFocus( mNative, true );
				mEventKeyLostFocus += value;
			}
			remove
			{
				mEventKeyLostFocus -= value;
				if (mEventKeyLostFocus == null) ExportWidgetEvent_AdviseKeyLostFocus( mNative, false );
			}
		}


		private struct ExportEventKeyLostFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateKeyLostFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.Interface)]  Widget _new );
				
			private static ExportHandle mDelegate;
			public ExportEventKeyLostFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateKeyLostFocus( mDelegate );
			}
		}
		static ExportEventKeyLostFocus mExportKeyLostFocus =
			new ExportEventKeyLostFocus(new ExportEventKeyLostFocus.ExportHandle( OnExportKeyLostFocus ));

		private static void OnExportKeyLostFocus(
			 Widget _sender ,
			 Widget _new )
		{
			if (_sender.mEventKeyLostFocus != null)
				_sender.mEventKeyLostFocus(
					 _sender ,
					 _new );
		}

		#endregion



   		#region Event MouseButtonDoubleClick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonDoubleClick( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonDoubleClick(
			 Widget _sender );
			
		private HandleMouseButtonDoubleClick mEventMouseButtonDoubleClick;
		public event HandleMouseButtonDoubleClick EventMouseButtonDoubleClick
		{
			add
			{
				if (mEventMouseButtonDoubleClick == null) ExportWidgetEvent_AdviseMouseButtonDoubleClick( mNative, true );
				mEventMouseButtonDoubleClick += value;
			}
			remove
			{
				mEventMouseButtonDoubleClick -= value;
				if (mEventMouseButtonDoubleClick == null) ExportWidgetEvent_AdviseMouseButtonDoubleClick( mNative, false );
			}
		}


		private struct ExportEventMouseButtonDoubleClick
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseButtonDoubleClick( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseButtonDoubleClick( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseButtonDoubleClick( mDelegate );
			}
		}
		static ExportEventMouseButtonDoubleClick mExportMouseButtonDoubleClick =
			new ExportEventMouseButtonDoubleClick(new ExportEventMouseButtonDoubleClick.ExportHandle( OnExportMouseButtonDoubleClick ));

		private static void OnExportMouseButtonDoubleClick(
			 Widget _sender )
		{
			if (_sender.mEventMouseButtonDoubleClick != null)
				_sender.mEventMouseButtonDoubleClick(
					 _sender );
		}

		#endregion



   		#region Event MouseButtonClick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonClick( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonClick(
			 Widget _sender );
			
		private HandleMouseButtonClick mEventMouseButtonClick;
		public event HandleMouseButtonClick EventMouseButtonClick
		{
			add
			{
				if (mEventMouseButtonClick == null) ExportWidgetEvent_AdviseMouseButtonClick( mNative, true );
				mEventMouseButtonClick += value;
			}
			remove
			{
				mEventMouseButtonClick -= value;
				if (mEventMouseButtonClick == null) ExportWidgetEvent_AdviseMouseButtonClick( mNative, false );
			}
		}


		private struct ExportEventMouseButtonClick
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseButtonClick( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseButtonClick( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseButtonClick( mDelegate );
			}
		}
		static ExportEventMouseButtonClick mExportMouseButtonClick =
			new ExportEventMouseButtonClick(new ExportEventMouseButtonClick.ExportHandle( OnExportMouseButtonClick ));

		private static void OnExportMouseButtonClick(
			 Widget _sender )
		{
			if (_sender.mEventMouseButtonClick != null)
				_sender.mEventMouseButtonClick(
					 _sender );
		}

		#endregion



   		#region Event MouseButtonReleased

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonReleased( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonReleased(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _id );
			
		private HandleMouseButtonReleased mEventMouseButtonReleased;
		public event HandleMouseButtonReleased EventMouseButtonReleased
		{
			add
			{
				if (mEventMouseButtonReleased == null) ExportWidgetEvent_AdviseMouseButtonReleased( mNative, true );
				mEventMouseButtonReleased += value;
			}
			remove
			{
				mEventMouseButtonReleased -= value;
				if (mEventMouseButtonReleased == null) ExportWidgetEvent_AdviseMouseButtonReleased( mNative, false );
			}
		}


		private struct ExportEventMouseButtonReleased
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseButtonReleased( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				  int _left ,
				  int _top ,
				[MarshalAs(UnmanagedType.I4)]  MouseButton _id );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseButtonReleased( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseButtonReleased( mDelegate );
			}
		}
		static ExportEventMouseButtonReleased mExportMouseButtonReleased =
			new ExportEventMouseButtonReleased(new ExportEventMouseButtonReleased.ExportHandle( OnExportMouseButtonReleased ));

		private static void OnExportMouseButtonReleased(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _id )
		{
			if (_sender.mEventMouseButtonReleased != null)
				_sender.mEventMouseButtonReleased(
					 _sender ,
					 _left ,
					 _top ,
					 _id );
		}

		#endregion



   		#region Event MouseButtonPressed

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonPressed( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonPressed(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _id );
			
		private HandleMouseButtonPressed mEventMouseButtonPressed;
		public event HandleMouseButtonPressed EventMouseButtonPressed
		{
			add
			{
				if (mEventMouseButtonPressed == null) ExportWidgetEvent_AdviseMouseButtonPressed( mNative, true );
				mEventMouseButtonPressed += value;
			}
			remove
			{
				mEventMouseButtonPressed -= value;
				if (mEventMouseButtonPressed == null) ExportWidgetEvent_AdviseMouseButtonPressed( mNative, false );
			}
		}


		private struct ExportEventMouseButtonPressed
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseButtonPressed( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				  int _left ,
				  int _top ,
				[MarshalAs(UnmanagedType.I4)]  MouseButton _id );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseButtonPressed( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseButtonPressed( mDelegate );
			}
		}
		static ExportEventMouseButtonPressed mExportMouseButtonPressed =
			new ExportEventMouseButtonPressed(new ExportEventMouseButtonPressed.ExportHandle( OnExportMouseButtonPressed ));

		private static void OnExportMouseButtonPressed(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _id )
		{
			if (_sender.mEventMouseButtonPressed != null)
				_sender.mEventMouseButtonPressed(
					 _sender ,
					 _left ,
					 _top ,
					 _id );
		}

		#endregion



   		#region Event MouseWheel

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseWheel( IntPtr _native, bool _advise );

		public delegate void HandleMouseWheel(
			 Widget _sender ,
			 int _rel );
			
		private HandleMouseWheel mEventMouseWheel;
		public event HandleMouseWheel EventMouseWheel
		{
			add
			{
				if (mEventMouseWheel == null) ExportWidgetEvent_AdviseMouseWheel( mNative, true );
				mEventMouseWheel += value;
			}
			remove
			{
				mEventMouseWheel -= value;
				if (mEventMouseWheel == null) ExportWidgetEvent_AdviseMouseWheel( mNative, false );
			}
		}


		private struct ExportEventMouseWheel
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseWheel( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				  int _rel );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseWheel( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseWheel( mDelegate );
			}
		}
		static ExportEventMouseWheel mExportMouseWheel =
			new ExportEventMouseWheel(new ExportEventMouseWheel.ExportHandle( OnExportMouseWheel ));

		private static void OnExportMouseWheel(
			 Widget _sender ,
			 int _rel )
		{
			if (_sender.mEventMouseWheel != null)
				_sender.mEventMouseWheel(
					 _sender ,
					 _rel );
		}

		#endregion



   		#region Event MouseMove

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseMove( IntPtr _native, bool _advise );

		public delegate void HandleMouseMove(
			 Widget _sender ,
			 int _left ,
			 int _top );
			
		private HandleMouseMove mEventMouseMove;
		public event HandleMouseMove EventMouseMove
		{
			add
			{
				if (mEventMouseMove == null) ExportWidgetEvent_AdviseMouseMove( mNative, true );
				mEventMouseMove += value;
			}
			remove
			{
				mEventMouseMove -= value;
				if (mEventMouseMove == null) ExportWidgetEvent_AdviseMouseMove( mNative, false );
			}
		}


		private struct ExportEventMouseMove
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseMove( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				  int _left ,
				  int _top );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseMove( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseMove( mDelegate );
			}
		}
		static ExportEventMouseMove mExportMouseMove =
			new ExportEventMouseMove(new ExportEventMouseMove.ExportHandle( OnExportMouseMove ));

		private static void OnExportMouseMove(
			 Widget _sender ,
			 int _left ,
			 int _top )
		{
			if (_sender.mEventMouseMove != null)
				_sender.mEventMouseMove(
					 _sender ,
					 _left ,
					 _top );
		}

		#endregion



   		#region Event MouseDrag

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseDrag( IntPtr _native, bool _advise );

		public delegate void HandleMouseDrag(
			 Widget _sender ,
			 int _left ,
			 int _top );
			
		private HandleMouseDrag mEventMouseDrag;
		public event HandleMouseDrag EventMouseDrag
		{
			add
			{
				if (mEventMouseDrag == null) ExportWidgetEvent_AdviseMouseDrag( mNative, true );
				mEventMouseDrag += value;
			}
			remove
			{
				mEventMouseDrag -= value;
				if (mEventMouseDrag == null) ExportWidgetEvent_AdviseMouseDrag( mNative, false );
			}
		}


		private struct ExportEventMouseDrag
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseDrag( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				  int _left ,
				  int _top );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseDrag( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseDrag( mDelegate );
			}
		}
		static ExportEventMouseDrag mExportMouseDrag =
			new ExportEventMouseDrag(new ExportEventMouseDrag.ExportHandle( OnExportMouseDrag ));

		private static void OnExportMouseDrag(
			 Widget _sender ,
			 int _left ,
			 int _top )
		{
			if (_sender.mEventMouseDrag != null)
				_sender.mEventMouseDrag(
					 _sender ,
					 _left ,
					 _top );
		}

		#endregion



   		#region Event MouseSetFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseSetFocus( IntPtr _native, bool _advise );

		public delegate void HandleMouseSetFocus(
			 Widget _sender ,
			 Widget _old );
			
		private HandleMouseSetFocus mEventMouseSetFocus;
		public event HandleMouseSetFocus EventMouseSetFocus
		{
			add
			{
				if (mEventMouseSetFocus == null) ExportWidgetEvent_AdviseMouseSetFocus( mNative, true );
				mEventMouseSetFocus += value;
			}
			remove
			{
				mEventMouseSetFocus -= value;
				if (mEventMouseSetFocus == null) ExportWidgetEvent_AdviseMouseSetFocus( mNative, false );
			}
		}


		private struct ExportEventMouseSetFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseSetFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.Interface)]  Widget _old );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseSetFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseSetFocus( mDelegate );
			}
		}
		static ExportEventMouseSetFocus mExportMouseSetFocus =
			new ExportEventMouseSetFocus(new ExportEventMouseSetFocus.ExportHandle( OnExportMouseSetFocus ));

		private static void OnExportMouseSetFocus(
			 Widget _sender ,
			 Widget _old )
		{
			if (_sender.mEventMouseSetFocus != null)
				_sender.mEventMouseSetFocus(
					 _sender ,
					 _old );
		}

		#endregion



   		#region Event MouseLostFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseLostFocus( IntPtr _native, bool _advise );

		public delegate void HandleMouseLostFocus(
			 Widget _sender ,
			 Widget _new );
			
		private HandleMouseLostFocus mEventMouseLostFocus;
		public event HandleMouseLostFocus EventMouseLostFocus
		{
			add
			{
				if (mEventMouseLostFocus == null) ExportWidgetEvent_AdviseMouseLostFocus( mNative, true );
				mEventMouseLostFocus += value;
			}
			remove
			{
				mEventMouseLostFocus -= value;
				if (mEventMouseLostFocus == null) ExportWidgetEvent_AdviseMouseLostFocus( mNative, false );
			}
		}


		private struct ExportEventMouseLostFocus
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateMouseLostFocus( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Widget _sender ,
				[MarshalAs(UnmanagedType.Interface)]  Widget _new );
				
			private static ExportHandle mDelegate;
			public ExportEventMouseLostFocus( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateMouseLostFocus( mDelegate );
			}
		}
		static ExportEventMouseLostFocus mExportMouseLostFocus =
			new ExportEventMouseLostFocus(new ExportEventMouseLostFocus.ExportHandle( OnExportMouseLostFocus ));

		private static void OnExportMouseLostFocus(
			 Widget _sender ,
			 Widget _new )
		{
			if (_sender.mEventMouseLostFocus != null)
				_sender.mEventMouseLostFocus(
					 _sender ,
					 _new );
		}

		#endregion



   		#region Method ClearUserStrings

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ClearUserStrings( IntPtr _native );

		public void ClearUserStrings( )
		{
			ExportWidget_ClearUserStrings(  mNative );
		}

		#endregion



   		#region Method IsUserString

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public bool IsUserString(
			string _key )
		{
			return ExportWidget_IsUserString_key( mNative , 
				 _key );
		}

		#endregion



   		#region Method ClearUserString

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_ClearUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public bool ClearUserString(
			string _key )
		{
			return ExportWidget_ClearUserString_key( mNative , 
				 _key );
		}

		#endregion



   		#region Method GetUserString

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportWidget_GetUserString_key( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key );

		public string GetUserString(
			string _key )
		{
			return ExportWidget_GetUserString_key( mNative , 
				 _key );
		}

		#endregion



   		#region Method SetUserString

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetUserString_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetUserString(
			string _key ,
			string _value )
		{
			ExportWidget_SetUserString_key_value( mNative , 
				 _key ,
				 _value );
		}

		#endregion



   		#region Method GetHeight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetHeight( IntPtr _native );

		public int GetHeight( )
		{
			return ExportWidget_GetHeight( mNative );
		}

		#endregion



   		#region Method GetWidth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetWidth( IntPtr _native );

		public int GetWidth( )
		{
			return ExportWidget_GetWidth( mNative );
		}

		#endregion



   		#region Method GetBottom

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetBottom( IntPtr _native );

		public int GetBottom( )
		{
			return ExportWidget_GetBottom( mNative );
		}

		#endregion



   		#region Method GetTop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetTop( IntPtr _native );

		public int GetTop( )
		{
			return ExportWidget_GetTop( mNative );
		}

		#endregion



   		#region Method GetRight

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetRight( IntPtr _native );

		public int GetRight( )
		{
			return ExportWidget_GetRight( mNative );
		}

		#endregion



   		#region Method GetLeft

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetLeft( IntPtr _native );

		public int GetLeft( )
		{
			return ExportWidget_GetLeft( mNative );
		}

		#endregion



   		#region Property Align

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportWidget_GetAlign( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align Align
		{
			get { return ExportWidget_GetAlign( mNative ); }
			set { ExportWidget_SetAlign( mNative,  value ); }
		}

		#endregion



   		#region Method GetAbsoluteTop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteTop( IntPtr _native );

		public int GetAbsoluteTop( )
		{
			return ExportWidget_GetAbsoluteTop( mNative );
		}

		#endregion



   		#region Method GetAbsoluteLeft

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportWidget_GetAbsoluteLeft( IntPtr _native );

		public int GetAbsoluteLeft( )
		{
			return ExportWidget_GetAbsoluteLeft( mNative );
		}

		#endregion



   		#region Method GetAbsoluteCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteCoord( IntPtr _native );

		public IntCoord GetAbsoluteCoord( )
		{
			return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetAbsoluteCoord(mNative), typeof(IntCoord));
		}

		#endregion



   		#region Method GetAbsoluteRect

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsoluteRect( IntPtr _native );

		public IntRect GetAbsoluteRect( )
		{
			return (IntRect)Marshal.PtrToStructure(ExportWidget_GetAbsoluteRect(mNative), typeof(IntRect));
		}

		#endregion



   		#region Method GetAbsolutePosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetAbsolutePosition( IntPtr _native );

		public IntPoint GetAbsolutePosition( )
		{
			return (IntPoint)Marshal.PtrToStructure(ExportWidget_GetAbsolutePosition(mNative), typeof(IntPoint));
		}

		#endregion



   		#region Property WidgetStyle

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern WidgetStyle ExportWidget_GetWidgetStyle( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetWidgetStyle( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  WidgetStyle _value );

		public WidgetStyle WidgetStyle
		{
			get { return ExportWidget_GetWidgetStyle( mNative ); }
			set { ExportWidget_SetWidgetStyle( mNative,  value ); }
		}

		#endregion



   		#region Method ChangeWidgetSkin

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ChangeWidgetSkin_skinname( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skinname );

		public void ChangeWidgetSkin(
			string _skinname )
		{
			ExportWidget_ChangeWidgetSkin_skinname( mNative , 
				 _skinname );
		}

		#endregion



   		#region Method AttachToWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_AttachToWidget_parent( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _parent );

		public void AttachToWidget(
			Widget _parent )
		{
			ExportWidget_AttachToWidget_parent( mNative , 
				 _parent );
		}

		#endregion



   		#region Method DetachFromWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_DetachFromWidget( IntPtr _native );

		public void DetachFromWidget( )
		{
			ExportWidget_DetachFromWidget(  mNative );
		}

		#endregion



   		#region Property EnableToolTip

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetEnableToolTip( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnableToolTip( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool EnableToolTip
		{
			get { return ExportWidget_GetEnableToolTip( mNative ); }
			set { ExportWidget_SetEnableToolTip( mNative,  value ); }
		}

		#endregion



   		#region Property NeedToolTip

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetNeedToolTip( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedToolTip( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedToolTip
		{
			get { return ExportWidget_GetNeedToolTip( mNative ); }
			set { ExportWidget_SetNeedToolTip( mNative,  value ); }
		}

		#endregion



   		#region Method GetClientWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetClientWidget( IntPtr _native );

		public Widget GetClientWidget( )
		{
			return ExportWidget_GetClientWidget( mNative );
		}

		#endregion



   		#region Method GetClientCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientCoord( IntPtr _native );

		public IntCoord GetClientCoord( )
		{
			return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetClientCoord(mNative), typeof(IntCoord));
		}

		#endregion



   		#region Property Pointer

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportWidget_GetPointer( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPointer( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string Pointer
		{
			get { return ExportWidget_GetPointer( mNative ); }
			set { ExportWidget_SetPointer( mNative,  value ); }
		}

		#endregion



   		#region Method SetEnabledSilent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabledSilent_enabled( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _enabled );

		public void SetEnabledSilent(
			bool _enabled )
		{
			ExportWidget_SetEnabledSilent_enabled( mNative , 
				 _enabled );
		}

		#endregion



   		#region Property Enabled

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsEnabled( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabled( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Enabled
		{
			get { return ExportWidget_IsEnabled( mNative ); }
			set { ExportWidget_SetEnabled( mNative,  value ); }
		}

		#endregion



   		#region Method SetMaskPick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetMaskPick_filename( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _filename );

		public void SetMaskPick(
			string _filename )
		{
			ExportWidget_SetMaskPick_filename( mNative , 
				 _filename );
		}

		#endregion



   		#region Property InheritsPick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsInheritsPick( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsPick( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsPick
		{
			get { return ExportWidget_IsInheritsPick( mNative ); }
			set { ExportWidget_SetInheritsPick( mNative,  value ); }
		}

		#endregion



   		#region Property NeedMouseFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsNeedMouseFocus( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedMouseFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedMouseFocus
		{
			get { return ExportWidget_IsNeedMouseFocus( mNative ); }
			set { ExportWidget_SetNeedMouseFocus( mNative,  value ); }
		}

		#endregion



   		#region Property NeedKeyFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsNeedKeyFocus( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedKeyFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedKeyFocus
		{
			get { return ExportWidget_IsNeedKeyFocus( mNative ); }
			set { ExportWidget_SetNeedKeyFocus( mNative,  value ); }
		}

		#endregion



   		#region Method FindWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_FindWidget_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public Widget FindWidget(
			string _name )
		{
			return ExportWidget_FindWidget_name( mNative , 
				 _name );
		}

		#endregion



   		#region Method GetChildAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetChildAt_index( IntPtr _native ,
			  uint _index );

		public Widget GetChildAt(
			uint _index )
		{
			return ExportWidget_GetChildAt_index( mNative , 
				 _index );
		}

		#endregion



   		#region Method GetChildCount

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportWidget_GetChildCount( IntPtr _native );

		public uint GetChildCount( )
		{
			return ExportWidget_GetChildCount( mNative );
		}

		#endregion



   		#region Method GetParent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetParent( IntPtr _native );

		public Widget GetParent( )
		{
			return ExportWidget_GetParent( mNative );
		}

		#endregion



   		#region Method IsRootWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsRootWidget( IntPtr _native );

		public bool IsRootWidget( )
		{
			return ExportWidget_IsRootWidget( mNative );
		}

		#endregion



   		#region Method SetState

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_SetState_state( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _state );

		public bool SetState(
			string _state )
		{
			return ExportWidget_SetState_state( mNative , 
				 _state );
		}

		#endregion



   		#region Property InheritsAlpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsInheritsAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsAlpha
		{
			get { return ExportWidget_IsInheritsAlpha( mNative ); }
			set { ExportWidget_SetInheritsAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Alpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportWidget_GetAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlpha( IntPtr _widget,   float _value );

		public float Alpha
		{
			get { return ExportWidget_GetAlpha( mNative ); }
			set { ExportWidget_SetAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Caption

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPWStr)]
		private static extern string ExportWidget_GetCaption( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCaption( IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public string Caption
		{
			get { return ExportWidget_GetCaption( mNative ); }
			set { ExportWidget_SetCaption( mNative,  value ); }
		}

		#endregion



   		#region Property Visible

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsVisible( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Visible
		{
			get { return ExportWidget_IsVisible( mNative ); }
			set { ExportWidget_SetVisible( mNative,  value ); }
		}

		#endregion



   		#region Method SetRealCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord_left_top_width_height( IntPtr _native ,
			  float _left ,
			  float _top ,
			  float _width ,
			  float _height );

		public void SetRealCoord(
			float _left ,
			float _top ,
			float _width ,
			float _height )
		{
			ExportWidget_SetRealCoord_left_top_width_height( mNative , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_width_height( IntPtr _native ,
			  float _width ,
			  float _height );

		public void SetRealSize(
			float _width ,
			float _height )
		{
			ExportWidget_SetRealSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetRealPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_left_top( IntPtr _native ,
			  float _left ,
			  float _top );

		public void SetRealPosition(
			float _left ,
			float _top )
		{
			ExportWidget_SetRealPosition_left_top( mNative , 
				 _left ,
				 _top );
		}

		#endregion



   		#region Method SetRealCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord_coord( IntPtr _native ,
			[In] ref FloatCoord _coord );

		public void SetRealCoord(
			FloatCoord _coord )
		{
			ExportWidget_SetRealCoord_coord( mNative , 
				ref _coord );
		}

		#endregion



   		#region Method SetRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_size( IntPtr _native ,
			[In] ref FloatSize _size );

		public void SetRealSize(
			FloatSize _size )
		{
			ExportWidget_SetRealSize_size( mNative , 
				ref _size );
		}

		#endregion



   		#region Method SetRealPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_point( IntPtr _native ,
			[In] ref FloatPoint _point );

		public void SetRealPosition(
			FloatPoint _point )
		{
			ExportWidget_SetRealPosition_point( mNative , 
				ref _point );
		}

		#endregion



   		#region Method SetCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord_left_top_width_height( IntPtr _native ,
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
			ExportWidget_SetCoord_left_top_width_height( mNative , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetSize(
			int _width ,
			int _height )
		{
			ExportWidget_SetSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition_left_top( IntPtr _native ,
			  int _left ,
			  int _top );

		public void SetPosition(
			int _left ,
			int _top )
		{
			ExportWidget_SetPosition_left_top( mNative , 
				 _left ,
				 _top );
		}

		#endregion



   		#region Property Coord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetCoord( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord( IntPtr _widget, [In] ref IntCoord _value );

		public IntCoord Coord
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetCoord(mNative), typeof(IntCoord)); }
			set { ExportWidget_SetCoord( mNative, ref value ); }
		}

		#endregion



   		#region Property Size

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize( IntPtr _widget,  ref IntSize _value );

		public IntSize Size
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportWidget_GetSize(mNative), typeof(IntSize)); }
			set { ExportWidget_SetSize( mNative, ref value ); }
		}

		#endregion



   		#region Property Position

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetPosition( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition( IntPtr _widget, [In] ref IntPoint _value );

		public IntPoint Position
		{
			get { return (IntPoint)Marshal.PtrToStructure(ExportWidget_GetPosition(mNative), typeof(IntPoint)); }
			set { ExportWidget_SetPosition( mNative, ref value ); }
		}

		#endregion



   		#region Method GetName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportWidget_GetName( IntPtr _native );

		public string GetName( )
		{
			return ExportWidget_GetName( mNative );
		}

		#endregion






   		#region Method CreateWidgetRealT

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_CreateWidgetRealT_type_skin_coord_align_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skin ,
			[In] ref FloatCoord _coord ,
			[MarshalAs(UnmanagedType.I4)]  Align _align ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public Widget CreateWidgetRealT(
			string _type ,
			string _skin ,
			FloatCoord _coord ,
			Align _align ,
			string _name )
		{
			return ExportWidget_CreateWidgetRealT_type_skin_coord_align_name( mNative , 
				 _type ,
				 _skin ,
				ref _coord ,
				 _align ,
				 _name );
		}

		#endregion


   		#region Method CreateWidgetRealT

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_CreateWidgetRealT_type_skin_coord_align( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _type ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skin ,
			[In] ref FloatCoord _coord ,
			[MarshalAs(UnmanagedType.I4)]  Align _align );

		public Widget CreateWidgetRealT(
			string _type ,
			string _skin ,
			FloatCoord _coord ,
			Align _align )
		{
			return ExportWidget_CreateWidgetRealT_type_skin_coord_align( mNative , 
				 _type ,
				 _skin ,
				ref _coord ,
				 _align );
		}

		#endregion


		
    }

}

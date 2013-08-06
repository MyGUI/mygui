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

    public partial class Widget
    {

		//InsertPoint
		#region Event RootKeyChangeFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseRootKeyChangeFocus( IntPtr _native, bool _advise );

		public delegate void HandleRootKeyChangeFocus(
			 Widget _sender ,
			 bool _focus );
			
		private HandleRootKeyChangeFocus mEventRootKeyChangeFocus;
		public event HandleRootKeyChangeFocus EventRootKeyChangeFocus
		{
			add
			{
				if (ExportEventRootKeyChangeFocus.mDelegate == null)
				{
					ExportEventRootKeyChangeFocus.mDelegate = new ExportEventRootKeyChangeFocus.ExportHandle( OnExportRootKeyChangeFocus );
					ExportEventRootKeyChangeFocus.ExportWidgetEvent_DelegateRootKeyChangeFocus( ExportEventRootKeyChangeFocus.mDelegate );
				}

				if (mEventRootKeyChangeFocus == null)
					ExportWidgetEvent_AdviseRootKeyChangeFocus( Native, true );
				mEventRootKeyChangeFocus += value;
			}
			remove
			{
				mEventRootKeyChangeFocus -= value;
				if (mEventRootKeyChangeFocus == null)
					ExportWidgetEvent_AdviseRootKeyChangeFocus( Native, false );
			}
		}

		private struct ExportEventRootKeyChangeFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateRootKeyChangeFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.U1)]  bool _focus );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseRootMouseChangeFocus( IntPtr _native, bool _advise );

		public delegate void HandleRootMouseChangeFocus(
			 Widget _sender ,
			 bool _focus );
			
		private HandleRootMouseChangeFocus mEventRootMouseChangeFocus;
		public event HandleRootMouseChangeFocus EventRootMouseChangeFocus
		{
			add
			{
				if (ExportEventRootMouseChangeFocus.mDelegate == null)
				{
					ExportEventRootMouseChangeFocus.mDelegate = new ExportEventRootMouseChangeFocus.ExportHandle( OnExportRootMouseChangeFocus );
					ExportEventRootMouseChangeFocus.ExportWidgetEvent_DelegateRootMouseChangeFocus( ExportEventRootMouseChangeFocus.mDelegate );
				}

				if (mEventRootMouseChangeFocus == null)
					ExportWidgetEvent_AdviseRootMouseChangeFocus( Native, true );
				mEventRootMouseChangeFocus += value;
			}
			remove
			{
				mEventRootMouseChangeFocus -= value;
				if (mEventRootMouseChangeFocus == null)
					ExportWidgetEvent_AdviseRootMouseChangeFocus( Native, false );
			}
		}

		private struct ExportEventRootMouseChangeFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateRootMouseChangeFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.U1)]  bool _focus );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeyButtonReleased( IntPtr _native, bool _advise );

		public delegate void HandleKeyButtonReleased(
			 Widget _sender ,
			 KeyCode _key );
			
		private HandleKeyButtonReleased mEventKeyButtonReleased;
		public event HandleKeyButtonReleased EventKeyButtonReleased
		{
			add
			{
				if (ExportEventKeyButtonReleased.mDelegate == null)
				{
					ExportEventKeyButtonReleased.mDelegate = new ExportEventKeyButtonReleased.ExportHandle( OnExportKeyButtonReleased );
					ExportEventKeyButtonReleased.ExportWidgetEvent_DelegateKeyButtonReleased( ExportEventKeyButtonReleased.mDelegate );
				}

				if (mEventKeyButtonReleased == null)
					ExportWidgetEvent_AdviseKeyButtonReleased( Native, true );
				mEventKeyButtonReleased += value;
			}
			remove
			{
				mEventKeyButtonReleased -= value;
				if (mEventKeyButtonReleased == null)
					ExportWidgetEvent_AdviseKeyButtonReleased( Native, false );
			}
		}

		private struct ExportEventKeyButtonReleased
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateKeyButtonReleased( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.I4)]  KeyCode _key );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
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
				if (ExportEventKeyButtonPressed.mDelegate == null)
				{
					ExportEventKeyButtonPressed.mDelegate = new ExportEventKeyButtonPressed.ExportHandle( OnExportKeyButtonPressed );
					ExportEventKeyButtonPressed.ExportWidgetEvent_DelegateKeyButtonPressed( ExportEventKeyButtonPressed.mDelegate );
				}

				if (mEventKeyButtonPressed == null)
					ExportWidgetEvent_AdviseKeyButtonPressed( Native, true );
				mEventKeyButtonPressed += value;
			}
			remove
			{
				mEventKeyButtonPressed -= value;
				if (mEventKeyButtonPressed == null)
					ExportWidgetEvent_AdviseKeyButtonPressed( Native, false );
			}
		}

		private struct ExportEventKeyButtonPressed
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateKeyButtonPressed( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.I4)]  KeyCode _key ,
				  uint _char );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeySetFocus( IntPtr _native, bool _advise );

		public delegate void HandleKeySetFocus(
			 Widget _sender ,
			 Widget _old );
			
		private HandleKeySetFocus mEventKeySetFocus;
		public event HandleKeySetFocus EventKeySetFocus
		{
			add
			{
				if (ExportEventKeySetFocus.mDelegate == null)
				{
					ExportEventKeySetFocus.mDelegate = new ExportEventKeySetFocus.ExportHandle( OnExportKeySetFocus );
					ExportEventKeySetFocus.ExportWidgetEvent_DelegateKeySetFocus( ExportEventKeySetFocus.mDelegate );
				}

				if (mEventKeySetFocus == null)
					ExportWidgetEvent_AdviseKeySetFocus( Native, true );
				mEventKeySetFocus += value;
			}
			remove
			{
				mEventKeySetFocus -= value;
				if (mEventKeySetFocus == null)
					ExportWidgetEvent_AdviseKeySetFocus( Native, false );
			}
		}

		private struct ExportEventKeySetFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateKeySetFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _old );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseKeyLostFocus( IntPtr _native, bool _advise );

		public delegate void HandleKeyLostFocus(
			 Widget _sender ,
			 Widget _new );
			
		private HandleKeyLostFocus mEventKeyLostFocus;
		public event HandleKeyLostFocus EventKeyLostFocus
		{
			add
			{
				if (ExportEventKeyLostFocus.mDelegate == null)
				{
					ExportEventKeyLostFocus.mDelegate = new ExportEventKeyLostFocus.ExportHandle( OnExportKeyLostFocus );
					ExportEventKeyLostFocus.ExportWidgetEvent_DelegateKeyLostFocus( ExportEventKeyLostFocus.mDelegate );
				}

				if (mEventKeyLostFocus == null)
					ExportWidgetEvent_AdviseKeyLostFocus( Native, true );
				mEventKeyLostFocus += value;
			}
			remove
			{
				mEventKeyLostFocus -= value;
				if (mEventKeyLostFocus == null)
					ExportWidgetEvent_AdviseKeyLostFocus( Native, false );
			}
		}

		private struct ExportEventKeyLostFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateKeyLostFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _new );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonDoubleClick( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonDoubleClick(
			 Widget _sender );
			
		private HandleMouseButtonDoubleClick mEventMouseButtonDoubleClick;
		public event HandleMouseButtonDoubleClick EventMouseButtonDoubleClick
		{
			add
			{
				if (ExportEventMouseButtonDoubleClick.mDelegate == null)
				{
					ExportEventMouseButtonDoubleClick.mDelegate = new ExportEventMouseButtonDoubleClick.ExportHandle( OnExportMouseButtonDoubleClick );
					ExportEventMouseButtonDoubleClick.ExportWidgetEvent_DelegateMouseButtonDoubleClick( ExportEventMouseButtonDoubleClick.mDelegate );
				}

				if (mEventMouseButtonDoubleClick == null)
					ExportWidgetEvent_AdviseMouseButtonDoubleClick( Native, true );
				mEventMouseButtonDoubleClick += value;
			}
			remove
			{
				mEventMouseButtonDoubleClick -= value;
				if (mEventMouseButtonDoubleClick == null)
					ExportWidgetEvent_AdviseMouseButtonDoubleClick( Native, false );
			}
		}

		private struct ExportEventMouseButtonDoubleClick
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseButtonDoubleClick( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMouseButtonDoubleClick(
			 Widget _sender )
		{
			if (_sender.mEventMouseButtonDoubleClick != null)
				_sender.mEventMouseButtonDoubleClick(
					 _sender );
		}

		#endregion
		#region Event MouseButtonClick

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseButtonClick( IntPtr _native, bool _advise );

		public delegate void HandleMouseButtonClick(
			 Widget _sender );
			
		private HandleMouseButtonClick mEventMouseButtonClick;
		public event HandleMouseButtonClick EventMouseButtonClick
		{
			add
			{
				if (ExportEventMouseButtonClick.mDelegate == null)
				{
					ExportEventMouseButtonClick.mDelegate = new ExportEventMouseButtonClick.ExportHandle( OnExportMouseButtonClick );
					ExportEventMouseButtonClick.ExportWidgetEvent_DelegateMouseButtonClick( ExportEventMouseButtonClick.mDelegate );
				}

				if (mEventMouseButtonClick == null)
					ExportWidgetEvent_AdviseMouseButtonClick( Native, true );
				mEventMouseButtonClick += value;
			}
			remove
			{
				mEventMouseButtonClick -= value;
				if (mEventMouseButtonClick == null)
					ExportWidgetEvent_AdviseMouseButtonClick( Native, false );
			}
		}

		private struct ExportEventMouseButtonClick
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseButtonClick( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMouseButtonClick(
			 Widget _sender )
		{
			if (_sender.mEventMouseButtonClick != null)
				_sender.mEventMouseButtonClick(
					 _sender );
		}

		#endregion
		#region Event MouseButtonReleased

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
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
				if (ExportEventMouseButtonReleased.mDelegate == null)
				{
					ExportEventMouseButtonReleased.mDelegate = new ExportEventMouseButtonReleased.ExportHandle( OnExportMouseButtonReleased );
					ExportEventMouseButtonReleased.ExportWidgetEvent_DelegateMouseButtonReleased( ExportEventMouseButtonReleased.mDelegate );
				}

				if (mEventMouseButtonReleased == null)
					ExportWidgetEvent_AdviseMouseButtonReleased( Native, true );
				mEventMouseButtonReleased += value;
			}
			remove
			{
				mEventMouseButtonReleased -= value;
				if (mEventMouseButtonReleased == null)
					ExportWidgetEvent_AdviseMouseButtonReleased( Native, false );
			}
		}

		private struct ExportEventMouseButtonReleased
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseButtonReleased( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				  int _left ,
				  int _top ,
				[MarshalAs(UnmanagedType.I4)]  MouseButton _id );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
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
				if (ExportEventMouseButtonPressed.mDelegate == null)
				{
					ExportEventMouseButtonPressed.mDelegate = new ExportEventMouseButtonPressed.ExportHandle( OnExportMouseButtonPressed );
					ExportEventMouseButtonPressed.ExportWidgetEvent_DelegateMouseButtonPressed( ExportEventMouseButtonPressed.mDelegate );
				}

				if (mEventMouseButtonPressed == null)
					ExportWidgetEvent_AdviseMouseButtonPressed( Native, true );
				mEventMouseButtonPressed += value;
			}
			remove
			{
				mEventMouseButtonPressed -= value;
				if (mEventMouseButtonPressed == null)
					ExportWidgetEvent_AdviseMouseButtonPressed( Native, false );
			}
		}

		private struct ExportEventMouseButtonPressed
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseButtonPressed( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				  int _left ,
				  int _top ,
				[MarshalAs(UnmanagedType.I4)]  MouseButton _id );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseWheel( IntPtr _native, bool _advise );

		public delegate void HandleMouseWheel(
			 Widget _sender ,
			 int _rel );
			
		private HandleMouseWheel mEventMouseWheel;
		public event HandleMouseWheel EventMouseWheel
		{
			add
			{
				if (ExportEventMouseWheel.mDelegate == null)
				{
					ExportEventMouseWheel.mDelegate = new ExportEventMouseWheel.ExportHandle( OnExportMouseWheel );
					ExportEventMouseWheel.ExportWidgetEvent_DelegateMouseWheel( ExportEventMouseWheel.mDelegate );
				}

				if (mEventMouseWheel == null)
					ExportWidgetEvent_AdviseMouseWheel( Native, true );
				mEventMouseWheel += value;
			}
			remove
			{
				mEventMouseWheel -= value;
				if (mEventMouseWheel == null)
					ExportWidgetEvent_AdviseMouseWheel( Native, false );
			}
		}

		private struct ExportEventMouseWheel
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseWheel( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				  int _rel );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
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
				if (ExportEventMouseMove.mDelegate == null)
				{
					ExportEventMouseMove.mDelegate = new ExportEventMouseMove.ExportHandle( OnExportMouseMove );
					ExportEventMouseMove.ExportWidgetEvent_DelegateMouseMove( ExportEventMouseMove.mDelegate );
				}

				if (mEventMouseMove == null)
					ExportWidgetEvent_AdviseMouseMove( Native, true );
				mEventMouseMove += value;
			}
			remove
			{
				mEventMouseMove -= value;
				if (mEventMouseMove == null)
					ExportWidgetEvent_AdviseMouseMove( Native, false );
			}
		}

		private struct ExportEventMouseMove
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseMove( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				  int _left ,
				  int _top );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseDrag( IntPtr _native, bool _advise );

		public delegate void HandleMouseDrag(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _value4 );
			
		private HandleMouseDrag mEventMouseDrag;
		public event HandleMouseDrag EventMouseDrag
		{
			add
			{
				if (ExportEventMouseDrag.mDelegate == null)
				{
					ExportEventMouseDrag.mDelegate = new ExportEventMouseDrag.ExportHandle( OnExportMouseDrag );
					ExportEventMouseDrag.ExportWidgetEvent_DelegateMouseDrag( ExportEventMouseDrag.mDelegate );
				}

				if (mEventMouseDrag == null)
					ExportWidgetEvent_AdviseMouseDrag( Native, true );
				mEventMouseDrag += value;
			}
			remove
			{
				mEventMouseDrag -= value;
				if (mEventMouseDrag == null)
					ExportWidgetEvent_AdviseMouseDrag( Native, false );
			}
		}

		private struct ExportEventMouseDrag
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseDrag( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				  int _left ,
				  int _top ,
				[MarshalAs(UnmanagedType.I4)]  MouseButton _value4 );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportMouseDrag(
			 Widget _sender ,
			 int _left ,
			 int _top ,
			 MouseButton _value4 )
		{
			if (_sender.mEventMouseDrag != null)
				_sender.mEventMouseDrag(
					 _sender ,
					 _left ,
					 _top ,
					 _value4 );
		}

		#endregion
		#region Event MouseSetFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseSetFocus( IntPtr _native, bool _advise );

		public delegate void HandleMouseSetFocus(
			 Widget _sender ,
			 Widget _old );
			
		private HandleMouseSetFocus mEventMouseSetFocus;
		public event HandleMouseSetFocus EventMouseSetFocus
		{
			add
			{
				if (ExportEventMouseSetFocus.mDelegate == null)
				{
					ExportEventMouseSetFocus.mDelegate = new ExportEventMouseSetFocus.ExportHandle( OnExportMouseSetFocus );
					ExportEventMouseSetFocus.ExportWidgetEvent_DelegateMouseSetFocus( ExportEventMouseSetFocus.mDelegate );
				}

				if (mEventMouseSetFocus == null)
					ExportWidgetEvent_AdviseMouseSetFocus( Native, true );
				mEventMouseSetFocus += value;
			}
			remove
			{
				mEventMouseSetFocus -= value;
				if (mEventMouseSetFocus == null)
					ExportWidgetEvent_AdviseMouseSetFocus( Native, false );
			}
		}

		private struct ExportEventMouseSetFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseSetFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _old );
				
			public static ExportHandle mDelegate;
		}

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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseMouseLostFocus( IntPtr _native, bool _advise );

		public delegate void HandleMouseLostFocus(
			 Widget _sender ,
			 Widget _new );
			
		private HandleMouseLostFocus mEventMouseLostFocus;
		public event HandleMouseLostFocus EventMouseLostFocus
		{
			add
			{
				if (ExportEventMouseLostFocus.mDelegate == null)
				{
					ExportEventMouseLostFocus.mDelegate = new ExportEventMouseLostFocus.ExportHandle( OnExportMouseLostFocus );
					ExportEventMouseLostFocus.ExportWidgetEvent_DelegateMouseLostFocus( ExportEventMouseLostFocus.mDelegate );
				}

				if (mEventMouseLostFocus == null)
					ExportWidgetEvent_AdviseMouseLostFocus( Native, true );
				mEventMouseLostFocus += value;
			}
			remove
			{
				mEventMouseLostFocus -= value;
				if (mEventMouseLostFocus == null)
					ExportWidgetEvent_AdviseMouseLostFocus( Native, false );
			}
		}

		private struct ExportEventMouseLostFocus
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateMouseLostFocus( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _new );
				
			public static ExportHandle mDelegate;
		}

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
		#region Method IsMaskPickInside

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsMaskPickInside_point_coord( IntPtr _native ,
			[In] ref IntPoint _point ,
			[In] ref IntCoord _coord );

		public bool IsMaskPickInside(
			IntPoint _point ,
			IntCoord _coord )
		{
			return  ExportWidget_IsMaskPickInside_point_coord( Native , 
				ref _point ,
				ref _coord )  ;
		}

		#endregion
		#region Property RootKeyFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetRootKeyFocus( IntPtr _native );

		public bool RootKeyFocus
		{
			get { return  ExportWidget_GetRootKeyFocus( Native )  ; }
		}

		#endregion
		#region Property RootMouseFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetRootMouseFocus( IntPtr _native );

		public bool RootMouseFocus
		{
			get { return  ExportWidget_GetRootMouseFocus( Native )  ; }
		}

		#endregion
		#region Property InheritsPick

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritsPick( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsPick( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsPick
		{
			get { return  ExportWidget_GetInheritsPick( Native )  ; }
			set { ExportWidget_SetInheritsPick( Native,  value ); }
		}

		#endregion
		#region Property NeedMouseFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetNeedMouseFocus( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedMouseFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedMouseFocus
		{
			get { return  ExportWidget_GetNeedMouseFocus( Native )  ; }
			set { ExportWidget_SetNeedMouseFocus( Native,  value ); }
		}

		#endregion
		#region Property NeedKeyFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetNeedKeyFocus( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedKeyFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedKeyFocus
		{
			get { return  ExportWidget_GetNeedKeyFocus( Native )  ; }
			set { ExportWidget_SetNeedKeyFocus( Native,  value ); }
		}

		#endregion
		#region Property Pointer

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetPointer( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPointer( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string Pointer
		{
			get { return  Marshal.PtrToStringAnsi(  ExportWidget_GetPointer( Native )  )  ; }
			set { ExportWidget_SetPointer( Native,  value ); }
		}

		#endregion
		#region Property NeedToolTip

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetNeedToolTip( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedToolTip( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedToolTip
		{
			get { return  ExportWidget_GetNeedToolTip( Native )  ; }
			set { ExportWidget_SetNeedToolTip( Native,  value ); }
		}

		#endregion
		
    }

}

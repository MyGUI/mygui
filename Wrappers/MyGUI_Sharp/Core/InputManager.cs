using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public class InputManager
	{
		#region Instance

		private static InputManager mInstance = new InputManager();

		public static InputManager Instance
		{
			get { return mInstance; }
		}

		#endregion

		#region KeyFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_GetKeyFocus();

		public bool KeyFocus
		{
			get { return ExportInputManager_GetKeyFocus(); }
		}

		#endregion

		#region KeyFocusWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportInputManager_GetKeyFocusWidget();
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportInputManager_SetKeyFocusWidget(IntPtr _widget);

		public Widget KeyFocusWidget
		{
			get { return (Widget)BaseWidget.GetByNative(ExportInputManager_GetKeyFocusWidget()); }
			set { ExportInputManager_SetKeyFocusWidget(value == null ? IntPtr.Zero : value.Native); }
		}

		#endregion

		#region MouseFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_GetMouseFocus();

		public bool MouseFocus
		{
			get { return ExportInputManager_GetMouseFocus(); }
		}

		#endregion

		#region MouseFocusWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportInputManager_GetMouseFocusWidget();

		public Widget MouseFocusWidget
		{
			get { return (Widget)BaseWidget.GetByNative(ExportInputManager_GetMouseFocusWidget()); }
		}

		#endregion

		#region ResetKeyFocus

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportInputManager_ResetKeyFocusWidget();

		public void ResetKeyFocus()
		{
			ExportInputManager_ResetKeyFocusWidget();
		}

		#endregion

		#region AddWidgetModal

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportInputManager_AddWidgetModal(IntPtr _widget);

		public void AddWidgetModal(Widget _widget)
		{
			ExportInputManager_AddWidgetModal(_widget.Native);
		}

		#endregion

		#region RemoveWidgetModal

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportInputManager_RemoveWidgetModal(IntPtr _widget);

		public void RemoveWidgetModal(Widget _widget)
		{
			ExportInputManager_RemoveWidgetModal(_widget.Native);
		}

		#endregion

		#region InjectKeyPress

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_InjectKeyPress(KeyCode _keyCode, uint _char);

		public bool InjectKeyPress(KeyCode _keyCode, uint _char)
		{
			return ExportInputManager_InjectKeyPress(_keyCode, _char);
		}

		#endregion

		#region InjectKeyRelease

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_InjectKeyRelease(KeyCode _keyCode);

		public bool InjectKeyRelease(KeyCode _keyCode)
		{
			return ExportInputManager_InjectKeyRelease(_keyCode);
		}

		#endregion

		#region InjectMousePress

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_InjectMousePress(int _absX, int _absY, MouseButton _mouseButton);

		public bool InjectMousePress(int _absX, int _absY, MouseButton _mouseButton)
		{
			return ExportInputManager_InjectMousePress(_absX, _absY, _mouseButton);
		}

		#endregion

		#region InjectMouseRelease

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton);

		public bool InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton)
		{
			return ExportInputManager_InjectMouseRelease(_absX, _absY, _mouseButton);
		}

		#endregion

		#region InjectMouseMove

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportInputManager_InjectMouseMove(int _absX, int _absY, int _absZ);

		public bool InjectMouseMove(int _absX, int _absY, int _absZ)
		{
			return ExportInputManager_InjectMouseMove(_absX, _absY, _absZ);
		}

		#endregion

		#region MousePosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportInputManager_GetMousePosition(
			[Out, MarshalAs(UnmanagedType.I4)] out int _x,
			[Out, MarshalAs(UnmanagedType.I4)] out int _y);

		public IntPoint MousePosition
		{
			get
			{
				IntPoint result = new IntPoint();
				ExportInputManager_GetMousePosition(out result.left, out result.top);
				return result;
			}
		}

		#endregion
	}
}

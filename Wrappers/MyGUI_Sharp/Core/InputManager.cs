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

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_GetKeyFocus();

        public bool KeyFocus
        {
            get { return ExportInputManager_GetKeyFocus(); }
        }

        #endregion

        #region KeyFocusWidget

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
        private static extern Widget ExportInputManager_GetKeyFocusWidget();
        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportInputManager_SetKeyFocusWidget(IntPtr _widget);

        public Widget KeyFocusWidget
        {
            get { return ExportInputManager_GetKeyFocusWidget(); }
            set { ExportInputManager_SetKeyFocusWidget(value == null ? IntPtr.Zero : value.GetNative()); }
        }

        #endregion

        #region MouseFocus

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_GetMouseFocus();

        public bool MouseFocus
        {
            get { return ExportInputManager_GetMouseFocus(); }
        }

        #endregion

        #region MouseFocusWidget

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
        private static extern Widget ExportInputManager_GetMouseFocusWidget();

        public Widget MouseFocusWidget
        {
            get { return ExportInputManager_GetMouseFocusWidget(); }
        }

        #endregion

        #region ResetKeyFocus

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportInputManager_ResetKeyFocusWidget();

        public void ResetKeyFocus()
        {
            ExportInputManager_ResetKeyFocusWidget();
        }

        #endregion

        #region AddWidgetModal

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportInputManager_AddWidgetModal(IntPtr _widget);

        public void AddWidgetModal(Widget _widget)
        {
            ExportInputManager_AddWidgetModal(_widget.GetNative());
        }

        #endregion

        #region RemoveWidgetModal

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportInputManager_RemoveWidgetModal(IntPtr _widget);

        public void RemoveWidgetModal(Widget _widget)
        {
            ExportInputManager_RemoveWidgetModal(_widget.GetNative());
        }

        #endregion

        #region InjectKeyPress

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectKeyPress(KeyCode _keyCode, uint _char);

        public bool InjectKeyPress(KeyCode _keyCode, uint _char)
        {
            return ExportInputManager_InjectKeyPress(_keyCode, _char);
        }

        #endregion

        #region InjectKeyRelease

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectKeyRelease(KeyCode _keyCode);

        public bool InjectKeyRelease(KeyCode _keyCode)
        {
            return ExportInputManager_InjectKeyRelease(_keyCode);
        }

        #endregion

        #region InjectMousePress

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMousePress(int _absX, int _absY, MouseButton _mouseButton);

        public bool InjectMousePress(int _absX, int _absY, MouseButton _mouseButton)
        {
            return ExportInputManager_InjectMousePress(_absX, _absY, _mouseButton);
        }

        #endregion

        #region InjectMouseRelease

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton);

        public bool InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton)
        {
            return ExportInputManager_InjectMouseRelease(_absX, _absY, _mouseButton);
        }

        #endregion

        #region InjectMouseMove

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMouseMove(int _absX, int _absY, int _absZ);

        public bool InjectMouseMove(int _absX, int _absY, int _absZ)
        {
            return ExportInputManager_InjectMouseMove(_absX, _absY, _absZ);
        }

        #endregion

    }
}

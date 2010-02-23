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

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_GetKeyFocus();

        public bool KeyFocus
        {
            get { return ExportInputManager_GetKeyFocus(); }
        }

        #endregion

        #region KeyFocusWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
        private static extern Widget ExportGui_GetKeyFocusWidget();
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_SetKeyFocusWidget(IntPtr _widget);

        public Widget KeyFocusWidget
        {
            get { return ExportGui_GetKeyFocusWidget(); }
            set { ExportGui_SetKeyFocusWidget(value == null ? IntPtr.Zero : value.GetNative()); }
        }

        #endregion

        #region MouseFocus

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportGui_GetMouseFocus();

        public bool MouseFocus
        {
            get { return ExportGui_GetMouseFocus(); }
        }

        #endregion

        #region MouseFocusWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
        private static extern Widget ExportGui_GetMouseFocusWidget();

        public Widget MouseFocusWidget
        {
            get { return ExportGui_GetMouseFocusWidget(); }
        }

        #endregion

        #region ResetKeyFocus

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_ResetKeyFocusWidget();

        public void ResetKeyFocus()
        {
            ExportGui_ResetKeyFocusWidget();
        }

        #endregion

        #region AddWidgetModal

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_AddWidgetModal(IntPtr _widget);

        public void AddWidgetModal(Widget _widget)
        {
            ExportGui_AddWidgetModal(_widget.GetNative());
        }

        #endregion

        #region RemoveWidgetModal

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportInputManager_RemoveWidgetModal(IntPtr _widget);

        public void RemoveWidgetModal(Widget _widget)
        {
            ExportInputManager_RemoveWidgetModal(_widget.GetNative());
        }

        #endregion

        #region InjectKeyPress

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectKeyPress(KeyCode _keyCode, uint _char);

        public bool InjectKeyPress(KeyCode _keyCode, uint _char)
        {
            return ExportInputManager_InjectKeyPress(_keyCode, _char);
        }

        #endregion

        #region InjectKeyRelease

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectKeyRelease(KeyCode _keyCode);

        public bool InjectKeyRelease(KeyCode _keyCode)
        {
            return ExportInputManager_InjectKeyRelease(_keyCode);
        }

        #endregion

        #region InjectMousePress

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMousePress(int _absX, int _absY, MouseButton _mouseButton);

        public bool InjectMousePress(int _absX, int _absY, MouseButton _mouseButton)
        {
            return ExportInputManager_InjectMousePress(_absX, _absY, _mouseButton);
        }

        #endregion

        #region InjectMouseRelease

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton);

        public bool InjectMouseRelease(int _absX, int _absY, MouseButton _mouseButton)
        {
            return ExportInputManager_InjectMouseRelease(_absX, _absY, _mouseButton);
        }

        #endregion

        #region InjectMouseMove

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportInputManager_InjectMouseMove(int _absX, int _absY, int _absZ);

        public bool InjectMouseMove(int _absX, int _absY, int _absZ)
        {
            return ExportInputManager_InjectMouseMove(_absX, _absY, _absZ);
        }

        #endregion

    }
}

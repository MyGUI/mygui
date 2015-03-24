using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public class PointerManager
    {

        #region Instance

        private static PointerManager mInstance = new PointerManager();

        public static PointerManager Instance
        {
            get { return mInstance; }
        }

        #endregion

        #region Visible

        [DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportPointerManager_SetVisible([MarshalAs(UnmanagedType.U1)]bool _value);
        [DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportPointerManager_GetVisible();

        public bool Visible
        {
            get { return ExportPointerManager_GetVisible(); }
            set { ExportPointerManager_SetVisible(value); }
        }

        #endregion

    }
}

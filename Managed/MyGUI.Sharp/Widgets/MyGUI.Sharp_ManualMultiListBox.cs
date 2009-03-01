using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class MultiListBox
    {

        #region Method GetItemDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMultiListBox_GetItemDataAt_index(IntPtr _widget,
             uint _index);

        public object GetItemDataAt(
            uint _index)
        {
            return ExportMultiListBox_GetItemDataAt_index(mNative,
                _index);
        }

        #endregion

        #region Method GetColumnDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMultiListBox_GetColumnDataAt_index(IntPtr _widget,
             uint _index);

        public object GetColumnDataAt(
            uint _index)
        {
            return ExportMultiListBox_GetColumnDataAt_index(mNative,
                _index);
        }

        #endregion

        #region Method GetSubItemDataAt

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.IUnknown)]
        private static extern object ExportMultiListBox_GetSubItemDataAt_index(IntPtr _widget,
             uint _column,
             uint _index);

        public object GetSubItemDataAt(
            uint _column,
            uint _index)
        {
            return ExportMultiListBox_GetSubItemDataAt_index(mNative,
                _column,
                _index);
        }

        #endregion

    }

}

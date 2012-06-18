﻿using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public class LayerManager
    {

        #region Instance

        private static LayerManager mInstance = new LayerManager();

        public static LayerManager Instance
        {
            get { return mInstance; }
        }

        #endregion

        #region AttachToLayer

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        private static extern void ExportLayerManager_AttachToLayer(
            [MarshalAs(UnmanagedType.LPStr)]  string _layer,
            IntPtr _widget);

        public void AttachToLayer(string _layer, Widget _widget)
        {
            ExportLayerManager_AttachToLayer(_layer, _widget.GetNative());
        }

        #endregion

        #region UpWidget

        [DllImport("MyGUI_Export", CallingConvention = CallingConvention.StdCall)]
        private static extern void ExportLayerManager_UpWidget(IntPtr _widget);

        public void UpWidget(Widget _widget)
        {
            ExportLayerManager_UpWidget(_widget.GetNative());
        }

        #endregion

    }
}

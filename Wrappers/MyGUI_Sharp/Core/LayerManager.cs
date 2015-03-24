using System;
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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportLayerManager_AttachToLayer(
			[MarshalAs(UnmanagedType.LPStr)]  string _layer,
			IntPtr _widget);

		public void AttachToLayer(string _layer, Widget _widget)
		{
			ExportLayerManager_AttachToLayer(_layer, _widget.Native);
		}

		#endregion

		#region UpWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportLayerManager_UpWidget(IntPtr _widget);

		public void UpWidget(Widget _widget)
		{
			ExportLayerManager_UpWidget(_widget.Native);
		}

		#endregion
	}
}

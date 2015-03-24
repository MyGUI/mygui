using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{
		#region Export

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderManager_SetViewSize(
			[MarshalAs(UnmanagedType.I4)] int _width,
			[MarshalAs(UnmanagedType.I4)] int _height);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderManager_DrawOneFrame();

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportRenderManager_GetBuffer(
			[MarshalAs(UnmanagedType.U4)] uint _index,
			[Out] out IntPtr _buffer,
			[Out, MarshalAs(UnmanagedType.U4)] out uint _countVertex,
			[Out, MarshalAs(UnmanagedType.U4)] out uint _vertexId,
			[Out, MarshalAs(UnmanagedType.U4)] out uint _textureId,
			[Out, MarshalAs(UnmanagedType.U4)] out uint _layerId,
			[Out, MarshalAs(UnmanagedType.U1)] out bool _change);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderManager_AddTexture(
			[MarshalAs(UnmanagedType.LPStr)] string _name,
			[MarshalAs(UnmanagedType.U4)] uint _id,
			[MarshalAs(UnmanagedType.I4)] int _width,
			[MarshalAs(UnmanagedType.I4)] int _height);

		#endregion

		public static void SetViewSize(int _width, int _height)
		{
			ExportRenderManager_SetViewSize(_width, _height);
		}

		public static void DrawOneFrame()
		{
			ExportRenderManager_DrawOneFrame();
		}

		public static bool GetBuffer(uint _index, out IntPtr _buffer, out uint _countVertex, out uint _vertexId, out uint _textureId, out uint _layerId, out bool _change)
		{
			return ExportRenderManager_GetBuffer(_index, out _buffer, out _countVertex, out _vertexId, out _textureId, out _layerId, out _change);
		}

		public static void AddTexture(string _name, uint _id, int _width, int _height)
		{
			ExportRenderManager_AddTexture(_name, _id, _width, _height);
		}
	}
}

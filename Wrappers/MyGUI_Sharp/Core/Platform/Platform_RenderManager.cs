using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{

		#region Export

		/*struct GetTextureSize
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegate([MarshalAs(UnmanagedType.LPStr)]string _name, [Out, In]ref int _width, [Out, In]ref int _height);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportRenderManager_DelegateTexture_LoadFromFile(HandleDelegate _delegate);

			private static void OnHandleDelegate(string _name, ref int _width, ref int _height)
			{
				mRenderManager.GetTextureSize(_name, out _width, out _height);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportRenderManager_DelegateTexture_LoadFromFile(mHandleDelegate);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportRenderManager_DelegateTexture_LoadFromFile(null);
				}
			}
		}*/

		/*struct VertexBuffer
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegate([Out, In] ref IntPtr _vertexes, [MarshalAs(UnmanagedType.U4)]uint _size);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportRenderManager_DelegateVertex_Lock(HandleDelegate _delegate);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegateUnlock([MarshalAs(UnmanagedType.U4)]uint _id);

			private static HandleDelegateUnlock mHandleDelegateUnlock;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportRenderManager_DelegateVertex_Unlock(HandleDelegateUnlock _delegate);

			private static void OnHandleDelegate(ref IntPtr _vertextes, uint _size)
			{
				mRenderManager.Lock(ref _vertextes, _size);
			}

			private static void OnHandleDelegateUnlock(uint _id)
			{
				mRenderManager.Unlock(_id);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportRenderManager_DelegateVertex_Lock(mHandleDelegate);

					mHandleDelegateUnlock += OnHandleDelegateUnlock;
					ExportRenderManager_DelegateVertex_Unlock(mHandleDelegateUnlock);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportRenderManager_DelegateVertex_Lock(null);

					mHandleDelegateUnlock -= OnHandleDelegateUnlock;
					ExportRenderManager_DelegateVertex_Unlock(null);
				}
			}
		}

		struct DoRender
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegate([MarshalAs(UnmanagedType.U4)]uint _bufferId, [MarshalAs(UnmanagedType.LPStr)]string _texture, [MarshalAs(UnmanagedType.U4)]uint _count);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportRenderManager_DelegateDoRender(HandleDelegate _delegate);

			private static void OnHandleDelegate(uint _bufferId, string _texture, uint _count)
			{
				mRenderManager.DoRender(_bufferId, _texture, _count);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportRenderManager_DelegateDoRender(mHandleDelegate);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportRenderManager_DelegateDoRender(null);
				}
			}
		}*/

		#endregion

		#region Export

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderManager_SetViewSize(
			[MarshalAs(UnmanagedType.I4)]int _width,
			[MarshalAs(UnmanagedType.I4)]int _height);

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderManager_DrawOneFrame();

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportRenderManager_GetBuffer(
			[MarshalAs(UnmanagedType.U4)]uint _index,
			[In, Out] ref IntPtr _buffer,
			[In, Out, MarshalAs(UnmanagedType.U4)] ref uint _countVertex,
			[In, Out, MarshalAs(UnmanagedType.U4)] ref uint _vertexId,
			[In, Out, MarshalAs(UnmanagedType.U4)] ref uint _textureId,
			[In, Out, MarshalAs(UnmanagedType.U1)] ref bool _change);

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		public static bool GetBuffer(uint _index, ref IntPtr _buffer, ref uint _countVertex, ref uint _vertexId, ref uint _textureId, ref bool _change)
		{
			return ExportRenderManager_GetBuffer(_index, ref _buffer, ref _countVertex, ref _vertexId, ref _textureId, ref _change);
		}

		public static void AddTexture(string _name, uint _id, int _width, int _height)
		{
			ExportRenderManager_AddTexture(_name, _id, _width, _height);
		}
	}
}

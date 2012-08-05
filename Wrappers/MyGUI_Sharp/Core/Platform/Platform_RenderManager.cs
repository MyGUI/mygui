using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public partial class Platform
	{

		#region Export

		struct GetTextureSize
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
		}

		/*struct GetDataPath
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.LPStr)]
			private delegate string HandleDelegate([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegate mHandleDelegate;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetDataPath(HandleDelegate _delegate);

			private static string OnHandleDelegate(string _name)
			{
				return mDataManager.GetDataPath(_name);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegate += OnHandleDelegate;
					ExportDataManager_DelegateGetDataPath(mHandleDelegate);
				}
				else
				{
					mHandleDelegate -= OnHandleDelegate;
					ExportDataManager_DelegateGetDataPath(null);
				}
			}
		}

		struct GetData
		{
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			[return: MarshalAs(UnmanagedType.U4)]
			private delegate uint HandleDelegateData([MarshalAs(UnmanagedType.LPStr)]string _name, [Out, In] ref IntPtr _data);

			private static HandleDelegateData mHandleDelegateData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateGetData(HandleDelegateData _delegate);

			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			private delegate void HandleDelegateFreeData([MarshalAs(UnmanagedType.LPStr)]string _name);

			private static HandleDelegateFreeData mHandleDelegateFreeData;
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportDataManager_DelegateFreeData(HandleDelegateFreeData _delegate);

			private static uint OnHandleDelegateData(string _name, ref IntPtr _data)
			{
				byte[] data = mDataManager.GetData(_name);
				if (data == null)
				{
					_data = IntPtr.Zero;
					return 0;
				}

				IntPtr ptr;
				if (mCashe.TryGetValue(_name, out ptr))
				{
					Marshal.FreeHGlobal(ptr);
					ptr = IntPtr.Zero;

					Log(LogLevel.Warning, string.Format("Cashe for data {0} already exist", _name));
				}

				ptr = Marshal.AllocHGlobal(data.Length);
				Marshal.Copy(data, 0, ptr, data.Length);
				_data = ptr;

				mCashe.Add(_name, ptr);

				return (uint)data.Length;
			}

			private static void OnHandleDelegateFreeData(string _name)
			{
				IntPtr ptr;
				if (mCashe.TryGetValue(_name, out ptr))
				{
					Marshal.FreeHGlobal(ptr);
					ptr = IntPtr.Zero;
				}
				else
				{
					Log(LogLevel.Warning, string.Format("Cashe for data {0} not found", _name));
				}

				mDataManager.FreeData(_name);
			}

			public static void Advise(bool _value)
			{
				if (_value)
				{
					mHandleDelegateData += OnHandleDelegateData;
					ExportDataManager_DelegateGetData(mHandleDelegateData);

					mHandleDelegateFreeData += OnHandleDelegateFreeData;
					ExportDataManager_DelegateFreeData(mHandleDelegateFreeData);
				}
				else
				{
					mHandleDelegateData -= OnHandleDelegateData;
					ExportDataManager_DelegateGetData(null);

					mHandleDelegateFreeData -= OnHandleDelegateFreeData;
					ExportDataManager_DelegateFreeData(null);
				}
			}

			private static Dictionary<string, IntPtr> mCashe = new Dictionary<string, IntPtr>();
		}*/


		#endregion

		private static void InitialiseRenderManager(IPlatformRenderManager _renderManager)
		{
			mRenderManager = _renderManager;

			GetTextureSize.Advise(true);
		}

		private static void ShutdownRenderManager()
		{
			GetTextureSize.Advise(false);

			mRenderManager = null;
		}

		private static IPlatformRenderManager mRenderManager;
	}
}

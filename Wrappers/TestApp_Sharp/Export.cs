using System.Runtime.InteropServices;

namespace TestApp
{
	public class Export
	{
		#region Export
#if DEBUG
		[DllImport("MyGUI_RenderWindow_Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Initialise();
		[DllImport("MyGUI_RenderWindow_Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_AddFrameDelegate(HandleFrameStart _delegate);
		[DllImport("MyGUI_RenderWindow_Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Run();
		[DllImport("MyGUI_RenderWindow_Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Shutdown();
#else
		[DllImport("MyGUI_RenderWindow_Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDemo_Initialise();
		[DllImport("MyGUI_RenderWindow_Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDemo_AddFrameDelegate(HandleFrameStart _delegate);
		[DllImport("MyGUI_RenderWindow_Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDemo_Run();
		[DllImport("MyGUI_RenderWindow_Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDemo_Shutdown();
#endif

		#endregion

		[UnmanagedFunctionPointer(CallingConvention.StdCall)]
		public delegate void HandleFrameStart(float _time);

		public static void Initialise()
		{
			ExportDemo_Initialise();
		}

		public static void AddFrameDelegate(HandleFrameStart _delegate)
		{
			ExportDemo_AddFrameDelegate(_delegate);
		}

		public static void Run()
		{
			ExportDemo_Run();
		}

		public static void Shutdown()
		{
			ExportDemo_Shutdown();
		}
	}
}

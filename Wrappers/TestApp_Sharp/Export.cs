using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace TestApp
{

    public class Export
    {
#if DEBUG
		private const string m_dllName = "MyGUI_RenderWindow_Export_d.dll";
#else
		private const string m_dllName = "MyGUI_RenderWindow_Export.dll";
#endif

		#region Export
		[DllImport(m_dllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Initialise();
		[DllImport(m_dllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_AddFrameDelegate(HandleFrameStart _delegate);
		[DllImport(m_dllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Run();
		[DllImport(m_dllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportDemo_Shutdown();

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

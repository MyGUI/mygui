using System;
using System.Runtime.InteropServices;
using Mogre;

namespace MyGUI.OgrePlatform
{
    public class Export
    {
        #region Export
#if DEBUG
        [DllImport("MyGUI.OgrePlatform.Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_CreateGUI();
        [DllImport("MyGUI.OgrePlatform.Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_DestroyGUI();
        [DllImport("MyGUI.OgrePlatform.Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetRenderWindow([MarshalAs(UnmanagedType.LPStr)]string _name);
        [DllImport("MyGUI.OgrePlatform.Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetSceneManager([MarshalAs(UnmanagedType.LPStr)]string _name);
        [DllImport("MyGUI.OgrePlatform.Export_d.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetActiveViewport(int _index);
#else
        [DllImport("MyGUI.OgrePlatform.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_CreateGUI();
        [DllImport("MyGUI.OgrePlatform.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_DestroyGUI();
        [DllImport("MyGUI.OgrePlatform.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetRenderWindow([MarshalAs(UnmanagedType.LPStr)]string _name);
        [DllImport("MyGUI.OgrePlatform.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetSceneManager([MarshalAs(UnmanagedType.LPStr)]string _name);
        [DllImport("MyGUI.OgrePlatform.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void Export_SetActiveViewport(int _index);
#endif
        #endregion

        public static void CreateGUI()
        {
            Export_CreateGUI();
        }
        public static void DestroyGUI()
        {
            Export_DestroyGUI();
        }

        public static void SetRenderWindow(RenderWindow _renderWindow)
        {
            Export_SetRenderWindow(_renderWindow.Name);
        }

        public static void SetSceneManager(SceneManager _sceneManager)
        {
            Export_SetSceneManager(_sceneManager.Name);
        }

        public static void SetActiveViewport(int _index)
        {
            Export_SetActiveViewport(_index);
        }
    }
}

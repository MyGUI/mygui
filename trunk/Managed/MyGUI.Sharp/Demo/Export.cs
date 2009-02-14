using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp.Demo
{

    public class Export
    {

        #region Export

        [DllImport("MyGUI.Export.dll")]
        public static extern void DemoExport_Initialise();
        [DllImport("MyGUI.Export.dll")]
        public static extern void DemoExport_Run();

        #endregion

        public delegate void HandleFrameStart(float _time);

        public static void Initialise()
        {
            DemoExport_Initialise();
        }

        public static void AddFrameDelegate(HandleFrameStart _delegate)
        {
        }

        public static void Run()
        {
            DemoExport_Run();
        }

    }

}

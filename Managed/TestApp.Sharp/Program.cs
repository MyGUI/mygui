using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    class Program
    {

        static void Main(string[] args)
        {
            Export.Initialise();

            Test_Button.Test();
            Test_Canvas.Test();
            Test_ComboBox.Test();
            Test_DDContainer.Test();
            Test_EditBox.Test();
            Test_ItemBox.Test();
            Test_ListBox.Test();
            Test_MenuCtrl.Test();

            Export.AddFrameDelegate(new MyGUI.Sharp.Demo.Export.HandleFrameStart(FrameStart));
            Export.Run();
        }

        static void FrameStart(float _time)
        {
        }

    }
}

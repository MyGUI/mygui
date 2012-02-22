using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Managed;
using MyGUI.Managed.Demo;

namespace TestApp.Sharp
{
    class Program
    {

        static void Main(string[] args)
        {
            Export.Initialise();

            Test_Button.Test();
            Test_ComboBox.Test();
            Test_DDContainer.Test();
            Test_EditBox.Test();
            Test_ItemBox.Test();
            Test_ListBox.Test();
            Test_MenuBar.Test();
            Test_MenuControl.Test();
            Test_MultiListBox.Test();
            Test_PopupMenu.Test();
            Test_ProgressBar.Test();
            Test_ScrollView.Test();
            Test_ImageBox.Test();
            Test_TextBox.Test();
            Test_TabControl.Test();
            Test_ScrollBar.Test();
            Test_Widget.Test();
            Test_Window.Test();

            Test_Gui.Test();

            Export.AddFrameDelegate(new MyGUI.Managed.Demo.Export.HandleFrameStart(FrameStart));
            Export.Run();
        }

        static float time = 0;
        static void FrameStart(float _time)
        {
            time += _time;
            if (time < 1) return;
            time = 0;

            Test_Gui.Update();
        }

    }
}

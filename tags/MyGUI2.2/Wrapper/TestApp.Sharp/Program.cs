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
            Test_HScrollBox.Test();
            Test_ItemBox.Test();
            Test_ListBox.Test();
            Test_MenuBar.Test();
            Test_MenuCtrl.Test();
            Test_MessageBox.Test();
            Test_MultiListBox.Test();
            Test_PopupMenu.Test();
            Test_ProgressBar.Test();
            Test_RenderBox.Test();
            Test_ScrollView.Test();
            Test_StaticImage.Test();
            Test_StaticText.Test();
            Test_TabBar.Test();
            Test_VScrollBar.Test();
            Test_Widget.Test();
            Test_Window.Test();

            Test_Gui.Test();

            Export.AddFrameDelegate(new MyGUI.Sharp.Demo.Export.HandleFrameStart(FrameStart));
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

using System;
using System.Collections.Generic;
using System.Text;
using MMyGUI;

namespace TestApp
{
    class Program
    {

        static void Main(string[] args)
        {
            demo.Export.Initialise();

            Initialise();

            demo.Export.AddFrameDelegate(new demo.Types.HandleFrameStart(FrameStart));
            demo.Export.Run();
        }

        static void Initialise()
        {
            Widget widget = new Widget("Button", new IntCoord(10, 10, 100, 100), new Align(20), "Overlapped");
            widget.MouseButtonClick += new Widget.HandleMouseButtonClick(widget_MouseButtonClick);
        }

        static void widget_MouseButtonClick(Widget _value1)
        {
            _value1.Dispose();
            demo.Export.Shutdown();
        }

        static void FrameStart(float _time)
        {
        }

    }
}

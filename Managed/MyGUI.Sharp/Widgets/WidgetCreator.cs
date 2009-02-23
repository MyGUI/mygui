using System;

namespace MyGUI.Sharp.Widgets
{

    internal class WidgetCreator
    {

        static public BaseWidget CreateWidget(BaseWidget _parent, string _type, IntPtr _widget)
        {
            if (_type == "StaticImage")
                return new StaticImage(_parent, _widget);
            else if (_type == "ComboBox")
                return new ComboBox(_parent, _widget);
            else if (_type == "DDContainer")
                return new DDContainer(_parent, _widget);
            return new Widget(_parent, _widget);
        }

    }

} // namespace MyGUI.Sharp.Widgets

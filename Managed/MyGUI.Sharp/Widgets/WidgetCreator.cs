using System;

namespace MyGUI.Sharp.Widgets
{

    internal class WidgetCreator
    {

        static public BaseWidget CreateWidget(BaseWidget _parent, string _type, IntPtr _widget)
        {
            if (_type == "Button")
                return new Button(_parent, _widget);
            else if (_type == "Canvas")
                return new Canvas(_parent, _widget);
            else if (_type == "ComboBox")
                return new ComboBox(_parent, _widget);
            else if (_type == "DDContainer")
                return new DDContainer(_parent, _widget);
            else if (_type == "Edit")
                return new EditBox(_parent, _widget);
            else if (_type == "HScroll")
                return new HScrollBar(_parent, _widget);
            else if (_type == "ItemBox")
                return new ItemBox(_parent, _widget);
            else if (_type == "List")
                return new ListBox(_parent, _widget);
            else if (_type == "MenuBar")
                return new MenuBar(_parent, _widget);
            else if (_type == "MenuCtrl")
                return new MenuCtrl(_parent, _widget);
            else if (_type == "Message")
                return new MessageBox(_parent, _widget);
            else if (_type == "MultiList")
                return new MultiListBox(_parent, _widget);
            else if (_type == "PopupMenu")
                return new PopupMenu(_parent, _widget);
            else if (_type == "Progress")
                return new ProgressBar(_parent, _widget);
            else if (_type == "RenderBox")
                return new RenderBox(_parent, _widget);
            else if (_type == "ScrollView")
                return new ScrollView(_parent, _widget);
            else if (_type == "StaticImage")
                return new StaticImage(_parent, _widget);
            else if (_type == "StaticText")
                return new StaticText(_parent, _widget);
            else if (_type == "Tab")
                return new TabBar(_parent, _widget);
            else if (_type == "VScroll")
                return new VScrollBar(_parent, _widget);
            else if (_type == "Widget")
                return new Widget(_parent, _widget);
            else if (_type == "Window")
                return new Window(_parent, _widget);
            else if (_type == "MenuItem")
                return new MenuItem(_parent, _widget);
            else if (_type == "TabItem")
                return new TabItem(_parent, _widget);

            return null;
        }

    }

} // namespace MyGUI.Sharp.Widgets

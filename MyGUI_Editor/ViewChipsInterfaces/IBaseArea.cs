using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using Common;

namespace ViewChipsInterfaces
{
    public interface IBaseArea
    {
        IItem AddItem(string _name, Image _image, Rectangle _clipRect, Align _align, object _data);
        IItem AddItem(string _name, Image _image, Align _align, object _data);
        IItem AddItem(IItem _item);

        IItem GetItem(string _name);
        IItem GetItem(Point _location, GetChildAtPointSkip _skip);
        IItem RemoveItem(string _name);
        IEnumerable<IItem> EnumItems { get; }
        IItem this[int _index] { get; }
        IItem this[string _name] { get; }
        int Count { get; }

        Point Location { get; set; }
        Size Size { get; set; }
        Rectangle Rect { get; set; }

        bool Visible { get; set; }
        bool Enabled { get; set; }
        string Name { get; set; }

        void Invalidate();
        void Invalidate(Rectangle _rec);

        void Dispose();
    }
}

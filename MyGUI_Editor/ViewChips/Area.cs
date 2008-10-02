using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using Core.Attributes;
using ViewChips;
using ViewChipsInterfaces;

namespace ViewChips
{
    [RegisterFactoryClass(CLSID_VIEWCHIP_AREA.Literal)]
    public partial class Area : UserControl/*PictureBox*/, IArea
    {
        protected bool m_Scaling;

        public Area()
        {
            InitializeComponent();
            BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
        }

        public IItem AddItem(string _name, Image _image, Rectangle _rect, Common.Align _align, object _data)
        {
            if (Controls.ContainsKey(_name)) throw new Exception("Имя контрола должно быть уникальным");
            Item item = new Item();
            item.Tag = _data;
            item.ClipRectangle = _rect;
            item.Image = _image;
            item.Align = _align;
            item.Name = _name;
            Controls.Add(item);
            return item;
        }

        public IItem AddItem(string _name, Image _image, Common.Align _align, object _data)
        {
            return AddItem(_name, _image, 
                new Rectangle(0, 0, _image == null ? 0 : _image.Size.Width, _image == null ? 0 : _image.Size.Height), 
                _align, _data);
        }

        public IItem AddItem(IItem _item)
        {
            if (Controls.ContainsKey(_item.Name)) throw new Exception("Имя контрола должно быть уникальным");
            if (!(_item is Control)) throw new Exception("Не верный аргумент");
            Controls.Add(_item as Control);
            return _item;
        }

        public IItem GetItem(string _name)
        {
            return Controls[_name] as IItem;
        }

        public IItem GetItem(Point _location, GetChildAtPointSkip _skip)
        {
            return GetChildAtPoint(_location, _skip) as IItem;
        }

        public IItem RemoveItem(string _name)
        {
            IItem itm = Controls[_name] as IItem;
            Controls.Remove(Controls[_name]);
            return itm;
        }

        public IEnumerable<IItem> EnumItems
        {
            get
            {
                foreach (Control control in Controls) 
                { if (control is IItem) yield return control as IItem; }
            }
        }

        public IItem this[int _index]
        {
            get { return Controls[_index] as IItem; }
        }

        public IItem this[string _name]
        {
            get { return Controls[_name] as IItem; }
        }

        public int Count
        {
            get { return Controls.Count; }
        }

        public Rectangle Rect
        {
            get { return new Rectangle(Location, Size); }
            set
            {
                Location = value.Location;
                Size = value.Size;
            }
        }

        internal bool Scaling
        {
            get
            {
                return m_Scaling;
            }
        }
    }
}
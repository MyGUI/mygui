using System.Drawing;
using Core.Attributes;
using ViewChipsInterfaces;
using Align = Common.Align;

namespace ViewChips
{
    [RegisterFactoryClass(CLSID_DEFAULT_ITEMCONTROLLER.Literal)]
    public class ItemController : Item, IItemController
    {
        private IItem m_Item;
        private IItem m_Selector;
        private IItem m_Controller;

        public ItemController()
        {
            Size = new Size(3, 3);
            Image sel = Image.FromHbitmap(Resource1.selector.GetHbitmap());

            m_Selector = AddItem("selector", null, Align.STRETCH, null);

            /*CreateItem(m_Selector, "1", sel, new Rectangle(0, 0, 1, 1), Align.LEFT | Align.TOP);
            CreateItem(m_Selector, "2", sel, new Rectangle(1, 0, 1, 1), Align.HSTRETCH | Align.TOP);
            CreateItem(m_Selector, "3", sel, new Rectangle(2, 0, 1, 1), Align.RIGHT| Align.TOP);
            CreateItem(m_Selector, "4", sel, new Rectangle(0, 1, 1, 1), Align.LEFT | Align.VSTRETCH);
            CreateItem(m_Selector, "5", sel, new Rectangle(1, 1, 1, 1), Align.HSTRETCH | Align.VSTRETCH);
            CreateItem(m_Selector, "6", sel, new Rectangle(2, 1, 1, 1), Align.RIGHT | Align.VSTRETCH);
            CreateItem(m_Selector, "7", sel, new Rectangle(0, 2, 1, 1), Align.LEFT | Align.BOTTOM);
            CreateItem(m_Selector, "8", sel, new Rectangle(1, 2, 1, 1), Align.HSTRETCH | Align.BOTTOM);
            CreateItem(m_Selector, "9", sel, new Rectangle(2, 2, 1, 1), Align.RIGHT | Align.BOTTOM);*/

            Size = new Size(50, 50);

            Image ctrl = Image.FromHbitmap(Resource1.controller.GetHbitmap());
            m_Controller = m_Selector.AddItem("controller", null, Align.STRETCH, null);

            CreateItem(m_Controller, "1", ctrl, new Rectangle(0, 0, 10, 10), Align.TOP | Align.LEFT);
            CreateItem(m_Controller, "2", ctrl, new Rectangle(20, 0, 10, 10), Align.TOP | Align.HCENTER);
            CreateItem(m_Controller, "3", ctrl, new Rectangle(40, 0, 10, 10), Align.TOP | Align.RIGHT);

            CreateItem(m_Controller, "4", ctrl, new Rectangle(0, 20, 10, 10), Align.HCENTER | Align.LEFT);
            CreateItem(m_Controller, "5", ctrl, new Rectangle(40, 20, 10, 10), Align.HCENTER | Align.RIGHT);

            CreateItem(m_Controller, "6", ctrl, new Rectangle(0, 40, 10, 10), Align.BOTTOM | Align.LEFT);
            CreateItem(m_Controller, "7", ctrl, new Rectangle(20, 40, 10, 10), Align.BOTTOM | Align.HCENTER);
            CreateItem(m_Controller, "8", ctrl, new Rectangle(40, 40, 10, 10), Align.BOTTOM | Align.RIGHT);
        }

        void CreateItem(IBaseArea _owner, string _name, Image _image, Rectangle _rect, Align _align)
        {
            _owner.AddItem(_name, _image, _rect, _align, null).Rect = _rect;
        }

        public IItem Item
        {
            get { return m_Item; }
            set
            {
                m_Item = value;
                m_Item.AddItem(m_Selector);
            }
        }
    }
}

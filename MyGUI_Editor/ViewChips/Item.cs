using System;
using System.Drawing;
using System.Windows.Forms;
using Core.Attributes;
using ViewChipsInterfaces;

namespace ViewChips
{
    [RegisterFactoryClass(CLSID_VIEWCHIP_ITEM.Literal)]
    public partial class Item : Area, IItem
    {
        private Size m_ParentSize;
        private int m_Scale = 1;
        private Image m_Image;
        private Common.Align m_Align;
        private Rectangle m_ClipRect;

        public Item()
        {
            InitializeComponent();
        }

        public Image Image
        {
            get { return m_Image; }
            set
            {
                m_Image = value;
                Invalidate();
            }
        }

        public Rectangle ClipRectangle
        {
            get { return m_ClipRect; }
            set { m_ClipRect = value; }
        }

        public Common.Align Align
        {
            get { return m_Align; }
            set { m_Align = value; }
        }

        public int ScaleKoof
        {
            get { return m_Scale; }
            set
            {
                m_Scaling = true;

                int old_w = Width;
                int old_h = Height;
                Size = new Size(value * Width / m_Scale, value * Height / m_Scale);
                Location = new Point(Left + (old_w - Width)/2, Top + (old_h - Height)/2);
                m_Scale = value;

                IItem item;
                foreach (Control control in Controls)
                    if ((item = control as IItem) != null) item.ScaleKoof = value;

                m_Scaling = false;
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_Image == null || (Parent is Area && (Parent as Area).Scaling))
            {
                base.OnPaint(e);
                return;
            }

            int dw = Parent.Size.Width - m_ParentSize.Width;
            int dh = Parent.Size.Height - m_ParentSize.Height;

            if((Align & Common.Align.RIGHT) != 0) Location = new Point(Location.X + dw, Location.Y);
            if ((Align & Common.Align.BOTTOM) != 0) Location = new Point(Location.X, Location.Y + dh);
            if ((Align & Common.Align.HSTRETCH) != 0) Size = new Size(Size.Width + dw, Size.Height);
            if ((Align & Common.Align.VSTRETCH) != 0) Size = new Size(Size.Width, Size.Height + dh);
            if ((Align & Common.Align.HCENTER) != 0) Location = new Point((m_ParentSize.Width - Size.Width) / 2, Location.Y);
            if ((Align & Common.Align.VCENTER) != 0) Location = new Point(Location.X, (m_ParentSize.Height - Size.Height) / 2);

            m_ParentSize = Parent.Size;
            e.Graphics.DrawImage(m_Image, new Rectangle(0, 0, ClientSize.Width, ClientSize.Height), m_ClipRect, GraphicsUnit.Pixel);
        }

        public void MoveToCenter()
        {
            Location = new Point((Parent.Width - Width) / 2, (Parent.Height - Height) / 2);
        }

        protected override void OnControlAdded(ControlEventArgs e)
        {
            base.OnControlAdded(e);
            m_ParentSize = e.Control.Size;
        }

        protected override void OnParentChanged(EventArgs e)
        {
            
        }

        public void ResizeToImage()
        {
            if(Image == null) return;
            Size = Image.Size;
        }

        public void SetSelector(Guid _id)
        {
            IItemController selector = Core.CoreFacade.Register.CreateItem<IItemController>(_id);
            if(selector == null) SetSelector(CLSID_DEFAULT_ITEMCONTROLLER.ID);
            selector.Item = this;
        }
    }
}

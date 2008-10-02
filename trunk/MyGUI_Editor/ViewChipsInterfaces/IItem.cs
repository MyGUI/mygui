using System;
using System.Drawing;
using ViewChipsInterfaces;

namespace ViewChipsInterfaces
{
    public interface IItem : IBaseArea
    {
        Image Image { get; set; }
        object Tag { get; set; }
        int ScaleKoof { get; set; }
        Common.Align Align { get; set; }
        void MoveToCenter();
        void ResizeToImage();

        void SetSelector(Guid _id);
    }
}

using System.Windows.Forms;

namespace ViewChipsInterfaces
{
    public interface IArea : IBaseArea
    {
        DockStyle Dock { get; set; }
        bool AutoScroll { get; set; }
    }
}
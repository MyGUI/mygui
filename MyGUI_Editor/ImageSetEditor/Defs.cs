
using System;
using Common;
using Core.Attributes;
using ViewChipsInterfaces;

namespace ImageSetEditor
{
    public struct CLSID_IMAGE_SET_EDITOR
    {
        public const string Literal = "519c8a43-484e-471e-b95f-2feecb960ef8";
        public static Guid ID = new Guid(Literal);
    }

    public class ElementRegister
    {
        [StartStaticMethod]
        public static void Register()
        {
            ElementTypeConvertor.WritePair("ImageChip", CLSID_VIEWCHIP_ITEM.ID);
        }
    }
}
using System;

namespace ViewChipsInterfaces
{
    public struct CLSID_VIEWCHIP_AREA
    {
        public const string Literal = "d97a47d7-b0eb-49f3-8360-6662a30541a0";
        public static Guid ID = new Guid(Literal);
    }

    public struct CLSID_VIEWCHIP_ITEM
    {
        public const string Literal = "9f7ba0db-c5df-4cfa-89c0-8d83165ce8d0";
        public static Guid ID = new Guid(Literal);
    }

    public struct CLSID_DEFAULT_ITEMCONTROLLER
    {
        public const string Literal = "5dcaf621-2436-4af9-90f1-92cb954bd791";
        public static Guid ID = new Guid(Literal);
    }
}

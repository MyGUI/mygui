
using System;
using Common;
using Core.Attributes;

namespace ResourceAnimationEditor
{
    public struct CLSID_RESOURCE_ANIMATION_EDITOR
    {
        public const string Literal = "DD27246A-5C37-4319-A8E1-7544DB93C65B";
        public static Guid ID = new Guid(Literal);
    }    

    //public struct CLSID_PROJECTELEMENT_WIDGETSKIN
    //{
    //    public const string Literal = "9d15625e-dbee-4302-9b59-20104cbdedf7";
    //    public static Guid ID = new Guid(Literal);
    //}

    //public struct CLSID_PROJECTELEMENT_WIDGETSKIN_CHILD
    //{
    //    public const string Literal = "cb38fa78-d8fe-4998-87c8-e872291dc42b";
    //    public static Guid ID = new Guid(Literal);
    //}

    //public struct CLSID_PROJELEMENT_PROPERTY
    //{
    //    public const string Literal = "3d1e97cc-16d5-42a4-a7ce-2f19e40ae33d";
    //    public static Guid ID = new Guid(Literal);
    //}

    //public struct CLSID_PROJELEMENT_BASISSKIN
    //{
    //    public const string Literal = "98303e67-ba1b-4e8b-a9f1-e1dd1de0264b";
    //    public static Guid ID = new Guid(Literal);
    //}

    //public struct CLSID_PROJELEMENT_BASISSKIN_STATE
    //{
    //    public const string Literal = "51df7a20-1188-4d2a-a893-4fb23148730f";
    //    public static Guid ID = new Guid(Literal);
    //}

    public class ElementRegister
    {
        [StartStaticMethod]
        public static void Register()
        {
            //ElementTypeConvertor.WritePair("Skin", CLSID_PROJECTELEMENT_WIDGETSKIN.ID);
            //ElementTypeConvertor.WritePair("Child", CLSID_PROJECTELEMENT_WIDGETSKIN_CHILD.ID);
            //ElementTypeConvertor.WritePair("Property", CLSID_PROJELEMENT_PROPERTY.ID);
            //ElementTypeConvertor.WritePair("BasisSkin", CLSID_PROJELEMENT_BASISSKIN.ID);
            //ElementTypeConvertor.WritePair("State", CLSID_PROJELEMENT_BASISSKIN_STATE.ID);
        }
    }
}

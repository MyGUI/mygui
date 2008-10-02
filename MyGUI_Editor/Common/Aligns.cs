using System;

namespace Common
{
    /// <summary>
    /// Выравнивания
    /// </summary>
    [Flags]
    public enum Align
    {
        DEFAULT = 0,
        LEFT = 1 << 1,
        RIGHT = 1 << 2,
        TOP = 1 << 3,
        BOTTOM = 1 << 4,
        VCENTER = 1 << 5,
        HCENTER = 1 << 6,
        CENTER = VCENTER | HCENTER,
        VSTRETCH = 1 << 7,
        HSTRETCH = 1 << 8,
        STRETCH = VSTRETCH | HSTRETCH
    }

    /// <summary>
    /// Выравнивание текста
    /// </summary>
    enum TextAlign
    {
        DEFAULT = Align.DEFAULT,
        CENTER = Align.CENTER,
        LEFT_TOP = Align.LEFT | Align.TOP,
        LEFT_BOTTOM = Align.LEFT | Align.BOTTOM,
        LEFT_VCENTER = Align.LEFT | Align.VCENTER,
        RIGHT_TOP = Align.RIGHT | Align.TOP,
        RIGHT_BOTTOM = Align.RIGHT | Align.BOTTOM,
        RIGHT_VCENTER = Align.RIGHT | Align.VCENTER,
        HCENTER_BOTTOM = Align.HCENTER | Align.BOTTOM,
        HCENTER_VCENTER = CENTER,
    }

    /// <summary>
    /// Положение стартовой точки
    /// </summary>
    enum AlignStartPoint
    {
        DEFAULT = Align.DEFAULT,
        LEFT = Align.LEFT,
        TOP = Align.TOP,
        RIGHT = Align.RIGHT,
        BOTTOM = Align.BOTTOM
    }

    /// <summary>
    /// Выравнивание виджета
    /// </summary>
    public enum WidgetAlign
    {
        DEFAULT = Align.DEFAULT,
        CENTER = Align.VCENTER | Align.HCENTER,
        LEFT_TOP = Align.LEFT | Align.TOP,
        LEFT_BOTTOM = Align.LEFT | Align.BOTTOM,
        LEFT_VCENTER = Align.LEFT | Align.VCENTER,
        RIGHT_TOP = Align.RIGHT | Align.TOP,
        RIGHT_BOTTOM = Align.RIGHT | Align.BOTTOM,
        RIGHT_VCENTER = Align.RIGHT | Align.VCENTER,
        HCENTER_BOTTOM = Align.HCENTER | Align.BOTTOM,
        HCENTER_VCENTER = CENTER,

        STRETCH = Align.STRETCH,
        LEFT_VSTRETCH = Align.LEFT | Align.VSTRETCH,
        RIGHT_VSTRETCH = Align.RIGHT | Align.VSTRETCH,
        HSTRETCH_TOP = Align.HSTRETCH | Align.TOP,
        HSTRETCH_BOTTOM = Align.HSTRETCH | Align.BOTTOM,
        HSTRETCH_VSTRETCH = Align.STRETCH,
        HSTRETCH_VCENTER = Align.HSTRETCH | Align.VCENTER,
        HCENTER_TOP = Align.HCENTER | Align.TOP,
        HCENTER_VSTRETCH  = Align.HCENTER | Align.VSTRETCH
    }
}
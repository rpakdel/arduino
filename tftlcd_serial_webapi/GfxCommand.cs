using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace tftlcd_serial_webapi4
{
    public enum GfxCommand
    {
        unknown = 0,
        print,
        println,
        setTextColor,
        setTextSize,
        setTextWrap,
        setCursor,
        drawPixel,
        drawLine,
        drawRect,
        fillRect,
        fillScreen,
        drawCircle,
        fillCircle,
        drawTriangle,
        fillTriangle,
        drawRoundRect,
        fillRoundRect,
        drawChar,
        setRotation
    };
}
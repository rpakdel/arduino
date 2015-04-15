using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace tftlcd_serial_webapi4
{
    public enum GfxCommand
    {
        unknown,
        fillScreen,
        print,
        println,
        setTextColor,
        setTextSize,
        setTextWrap,
        setCursor,
        drawPixel,
        drawLine
    };
}
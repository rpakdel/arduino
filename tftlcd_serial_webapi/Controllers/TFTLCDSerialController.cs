using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace tftlcd_serial_webapi4.Controllers
{
    [RoutePrefix("api/tft")]
    public class TFTLCDSerialController : ApiController
    {
        static TFTLCD tft = new TFTLCD("COM5");

        [Route("init/{portNumber:int}")]
        [HttpGet]
        public bool initialize(int portNumber)
        {
            tft = new TFTLCD("COM" + portNumber);
            return true;
        }

        [Route("gfx/fillscreen/{colorName}")]
        [HttpGet]
        public void fillScreen(string colorName)
        {
            tft.ExecGfxCommand(GfxCommand.fillScreen, colorName);
        }

        [Route("gfx/print/{str}")]
        [HttpGet]
        public void print(string str)
        {
            tft.ExecGfxCommand(GfxCommand.print, str);
        }

        [Route("gfx/println/{str}")]
        [HttpGet]
        public void println(string str)
        {
            tft.ExecGfxCommand(GfxCommand.println, str);
        }

        [Route("gfx/setcursor/{x:int}/{y:int}")]
        [HttpGet]
        public void setCursor(int x, int y)
        {
            tft.ExecGfxCommand(GfxCommand.setCursor, x, y);
        }

        [Route("gfx/drawpixel/{x:int}/{y:int}/{colorName}")]
        [HttpGet]
        public void drawPixel(int x, int y, string colorName)
        {
            tft.ExecGfxCommand(GfxCommand.drawPixel, x, y, colorName);
        }

        [Route("gfx/drawline/{x0:int}/{y0:int}/{x1:int}/{y1:int}/{colorName}")]
        [HttpGet]
        public void drawLine(int x0, int y0, int x1, int y1, string colorName)
        {
            tft.ExecGfxCommand(GfxCommand.drawLine, x0, y0, x1, y1, colorName);
        }
    }
}

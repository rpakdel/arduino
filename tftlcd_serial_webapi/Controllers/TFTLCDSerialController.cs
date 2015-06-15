using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web.Http;

namespace tftlcd_serial_webapi4.Controllers
{
    [RoutePrefix("api/tft")]
    public class TFTLCDSerialController : ApiController
    {
        static TFTLCD tft = new TFTLCD("COM5", 115200);

        Task<IHttpActionResult> ExecGfxCommand(GfxCommand command, params object[] parameters)
        {
            Task<IHttpActionResult> t = Task<IHttpActionResult>.Factory.StartNew(() =>
            {
                try
                {
                    string commandString = tft.CreateCommand(command, parameters);
                    tft.WriteCommand(commandString);
                    return Ok();
                }
                catch (System.Exception ex)
                {
                    return InternalServerError(ex);
                }
            });
            return t;
        }

        [Route("gfx/fillscreen/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> fillScreen(string colorName)
        {
            return ExecGfxCommand(GfxCommand.fillScreen, colorName);
        }

        [Route("gfx/print/{str}")]
        [HttpGet]
        public Task<IHttpActionResult> print(string str)
        {
            return ExecGfxCommand(GfxCommand.print, str);
        }

        [Route("gfx/println/{str}")]
        [HttpGet]
        public Task<IHttpActionResult> println(string str)
        {
            return ExecGfxCommand(GfxCommand.println, str);
        }

        [Route("gfx/setcursor/{x:int}/{y:int}")]
        [HttpGet]
        public Task<IHttpActionResult> setCursor(int x, int y)
        {
            return ExecGfxCommand(GfxCommand.setCursor, x, y);
        }

        [Route("gfx/drawpixel/{x:int}/{y:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> drawPixel(int x, int y, string colorName)
        {
            return ExecGfxCommand(GfxCommand.drawPixel, x, y, colorName);
        }

        [Route("gfx/drawline/{x0:int}/{y0:int}/{x1:int}/{y1:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> drawLine(int x0, int y0, int x1, int y1, string colorName)
        {
            return ExecGfxCommand(GfxCommand.drawLine, x0, y0, x1, y1, colorName);
        }

        [Route("gfx/drawcircle/{x0:int}/{y0:int}/{r:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> drawCircle(int x0, int y0, int r, string colorName)
        {
            return ExecGfxCommand(GfxCommand.drawCircle, x0, y0, r, colorName);
        }

        [Route("gfx/fillcircle/{x0:int}/{y0:int}/{r:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> fillCircle(int x0, int y0, int r, string colorName)
        {
            return ExecGfxCommand(GfxCommand.fillCircle, x0, y0, r, colorName);
        }

        [Route("gfx/drawrect/{x:int}/{y:int}/{w:int}/{h:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> drawRect(int x, int y, int w, int h, string colorName)
        {
            return ExecGfxCommand(GfxCommand.drawRect, x, y, w, h, colorName);
        }

        [Route("gfx/fillrect/{x:int}/{y:int}/{w:int}/{h:int}/{colorName}")]
        [HttpGet]
        public Task<IHttpActionResult> fillRect(int x, int y, int w, int h, string colorName)
        {
            return ExecGfxCommand(GfxCommand.fillRect, x, y, w, h, colorName);
        }
    }
}


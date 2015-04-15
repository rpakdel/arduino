using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using System.IO.Ports;
using System.Threading;
using System.Text;

namespace tftlcd_serial_webapi4
{
    public class TFTLCD : IDisposable
    {
        SerialPort mSerialPort;

        public TFTLCD(string portName)
        {
            mSerialPort = new SerialPort(portName, 115200);
            mSerialPort.Open();
            Thread.Sleep(1000);
        }

        public void Dispose()
        {
            if (mSerialPort != null)
            {
                mSerialPort.Close();
                mSerialPort.Dispose();
                mSerialPort = null;
            }
        }

        public void ExecGfxCommand(GfxCommand command, params object[] parameters)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(command.ToString());
            sb.Append(":");
            if (parameters.Length > 0)
            {
                if (parameters.Length > 1)
                {
                    for (int i = 0; i < parameters.Length - 1; ++i)
                    {
                        sb.Append(parameters[i].ToString());
                        sb.Append(",");
                    }
                }
                sb.Append(parameters.Last().ToString());
            }
            mSerialPort.WriteLine(sb.ToString());
        }
    }
}
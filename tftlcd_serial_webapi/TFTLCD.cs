using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using System.IO.Ports;
using System.Threading;
using System.Text;
using System.ComponentModel;
using System.Collections.Concurrent;

using System.Threading.Tasks.Dataflow;
using System.Threading.Tasks;

namespace tftlcd_serial_webapi4
{
    public class TFTLCD : IDisposable
    {
        #region Fields

        SerialPort mSerialPort;
        BackgroundWorker mBackgroundWoker = new BackgroundWorker();
        ConcurrentQueue<string> mCommandQueue = new ConcurrentQueue<string>();
        object mSerialPortLock = new object();

        #endregion

        #region Properties

        public string PortName {get;protected set;}
        public int BaudRate { get; protected set; }

        public bool IsAvailable 
        { 
            get 
            { 
                return GetIsAvailable(); 
            } 
        }

        #endregion

        public void Open()
        {
            lock(mSerialPortLock)
            {
                OpenNoLock();
            }
        }

        void OpenNoLock()
        {
            CloseNoLock();

            mSerialPort = new SerialPort(this.PortName, this.BaudRate);
            mSerialPort.Open();
        }

        public bool GetIsAvailable()
        {
            lock(mSerialPortLock)
            {
                return GetIsAvailableNoLock();
            }
        }

        bool GetIsAvailableNoLock()
        {
            return mSerialPort != null && mSerialPort.IsOpen;
        }

        public void Close()
        {
            lock(mSerialPortLock)
            {
                CloseNoLock();
            }
        }

        void CloseNoLock()
        {
            if (mSerialPort != null)
            {
                if (mSerialPort.IsOpen)
                {
                    mSerialPort.Close();
                }

                mSerialPort.Dispose();
                mSerialPort = null;
            }
        }

        #region Constructor

        public TFTLCD(string portName, int baudRate)
        {
            this.PortName = portName;
            this.BaudRate = baudRate;
            mBackgroundWoker.DoWork += mBackgroundWoker_DoWork;
            mBackgroundWoker.RunWorkerAsync();
        }

        #endregion

        void mBackgroundWoker_DoWork(object sender, DoWorkEventArgs e)
        {
            while(true)
            {
                string command = null;
                if (mCommandQueue.TryDequeue(out command))
                {
                    lock (mSerialPortLock)
                    {
                        if (!GetIsAvailableNoLock())
                        {
                            OpenNoLock();
                        }

                        mSerialPort.WriteLine(command);
                    }
                }
                Thread.Sleep(10);
            }
        }

        public void Dispose()
        {
            Close();
        }



        public void WriteCommand(string command)
        {
            lock (mSerialPortLock)
            {
                if (!GetIsAvailableNoLock())
                {
                    OpenNoLock();
                }

                mSerialPort.WriteLine(command);
            }
        }

        public string CreateCommand(GfxCommand command, params object[] parameters)
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
            return sb.ToString();
        }
    }
}
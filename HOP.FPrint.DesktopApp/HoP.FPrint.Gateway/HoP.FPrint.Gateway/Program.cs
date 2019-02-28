using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;

namespace HoP.FPrint.Gateway
{
    public class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            foreach (var pName in SerialPort.GetPortNames())
            {
                Console.WriteLine(pName);
            }
            
            ConnectToPort("COM6");
            ConnectToPort("COM3");
            

            Console.ReadKey();
        }

        
        public static void ConnectToPort(string portName)
        {
            var serialPort = new SerialPort(portName)
            {
                BaudRate = 9600,
                Parity = Parity.None,
                StopBits = StopBits.One,
                DataBits = 8,
                Handshake = Handshake.None
            };

            serialPort.DataReceived += SerialPortDataReciever;
           
            
            serialPort.Open();
            Thread.Sleep(3000);
            serialPort.WriteLine("a");
        }

        private static void SerialPortDataReciever(object sender, SerialDataReceivedEventArgs e)
        {
            var serialPort = (SerialPort) sender;

            var serialdata = serialPort.ReadExisting();

            Console.WriteLine(serialdata);

            var url = $"https://hop-fp.visma.lv/{serialdata}";
            
            var prs = new ProcessStartInfo("cmd", $"/c start {url}");
            
            Process.Start(prs);
        }
    }
}
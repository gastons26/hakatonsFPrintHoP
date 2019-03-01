using System;
using System.Diagnostics;
using System.IO.Ports;
using HoP.FPrint.Gateway.FPrintSerialPort.Interfaces;

namespace HoP.FPrint.Gateway.FPrintSerialPort.Services
{
    public class FPrintSerialPortService : IFPrintSerialPortService
    {
        private readonly SerialPort _serialPort;

        public FPrintSerialPortService(string portName)
        {
            _serialPort = new SerialPort(portName){
                BaudRate = 9600,
                Parity = Parity.None,
                StopBits = StopBits.One,
                DataBits = 8,
                Handshake = Handshake.None
            };
            _serialPort.DataReceived += SerialPortDataReceiver;
        }
        
        private static void SerialPortDataReceiver(object sender, SerialDataReceivedEventArgs e)
        {
            var serialPort = (SerialPort) sender;
            OpenHopUrl(serialPort.ReadExisting());            
        }

        private static void OpenHopUrl(string userId)
        {
            var time = DateTime.UtcNow.AddSeconds(10).Ticks;

            var result = $"{userId}_{time}";
            
            var url = $"https://hop-fp.visma.lv/#!/fingerprint/{EncodeTo64(result)}";
            
            var prs = new ProcessStartInfo("cmd", $"/c start {url}");
            
            Process.Start(prs);
        }
        
        private static string EncodeTo64(string toEncode)
        {
            
            byte[] toEncodeAsBytes = System.Text.Encoding.ASCII.GetBytes($"{toEncode}");
            return Convert.ToBase64String(toEncodeAsBytes);
        }

        public void Open()
        {
            _serialPort.Open();
        }

        public void SendLineToSerialPort(string lineText)
        {
            _serialPort.WriteLine(lineText);
        }
    }
}
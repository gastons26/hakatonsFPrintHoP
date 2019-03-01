using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq.Expressions;
using System.Threading;
using HoP.FPrint.Gateway.FPrintSerialPort.Enums;

namespace HoP.FPrint.Gateway.FPrintSerialPort.Services
{
    public class FPrintSerialPortService
    {
        private static SerialPort _serialPort;
        private static IList<string> _portalMessages = new List<string>();

        private static FPrintOperationTypes _operationType = FPrintOperationTypes.R;

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

        public static void AddMessage(string message)
        {
            _portalMessages.Add(message);
        }


        public static IList<string> GetMessages()
        {
            var messages = new List<string>(_portalMessages); 
            
            _portalMessages.Clear();

            return messages;
        }
        private static void SerialPortDataReceiver(object sender, SerialDataReceivedEventArgs e)
        {
            var serialPort = (SerialPort) sender;
            
            switch (_operationType)
            {
                case FPrintOperationTypes.R:
                    OpenHopUrl(serialPort.ReadExisting());
                    break;
                case FPrintOperationTypes.C:
                    var message  = _serialPort.ReadLine();

                    Console.WriteLine(message);
                    _portalMessages.Add(message);
                    
                    if (message.StartsWith("I:Stored!"))
                    {
                        Thread.Sleep(2000);
                        _operationType = FPrintOperationTypes.R;
                    }
                    break;
            }         
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

        public static void SetOperation(FPrintOperationTypes operationType)
        {
            _operationType = operationType; 
            Console.WriteLine($"{_operationType}");
            _serialPort.WriteLine($"{_operationType}");
        }

        public static void SendMessageToSerialPort(string message)
        {
            Console.WriteLine($"{message}");
            _serialPort.WriteLine(message);
        }
    }
}
using HoP.FPrint.Gateway.AppStart;
using HoP.FPrint.Gateway.FPrintSerialPort.Services;
using Microsoft.AspNetCore.Hosting;

namespace HoP.FPrint.Gateway
{
    public class Program
    {
        static void Main(string[] args)
        {
            new FPrintSerialPortService("COM3").Open();
            
            var host = new WebHostBuilder()
                .UseKestrel()
                .UseIISIntegration()
                .UseStartup<StartUp>().Build();
            
            host.Run();
        }

    }
}
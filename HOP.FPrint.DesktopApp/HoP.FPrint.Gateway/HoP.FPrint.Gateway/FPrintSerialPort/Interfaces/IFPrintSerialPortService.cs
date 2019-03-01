namespace HoP.FPrint.Gateway.FPrintSerialPort.Interfaces
{
    public interface IFPrintSerialPortService
    {
        void Open();
        void SendLineToSerialPort(string lineText);
    }
}
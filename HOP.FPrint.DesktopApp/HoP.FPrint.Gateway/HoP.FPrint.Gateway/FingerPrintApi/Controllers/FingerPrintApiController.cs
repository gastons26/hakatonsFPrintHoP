using HoP.FPrint.Gateway.FPrintSerialPort.Enums;
using HoP.FPrint.Gateway.FPrintSerialPort.Services;
using Microsoft.AspNetCore.Mvc;

namespace HoP.FPrint.Gateway.FingerPrintApi.Controllers
{
    [Route("api/fingerprint")]
    public class FingerPrintApiController : Controller
    {
        
        [HttpGet]
        [Route("set")]
        public IActionResult Index()
        {
            FPrintSerialPortService.SetOperation(FPrintOperationTypes.C);
            FPrintSerialPortService.SendMessageToSerialPort("21");
            return Ok("Start");
        }
        
        [HttpGet]
        [Route("messages")]
        public IActionResult GetMessages()
        {
            return Ok(FPrintSerialPortService.GetMessages());
        }
    }
}
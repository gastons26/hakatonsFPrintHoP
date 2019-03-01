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
            return Ok("Thanks, we recieved your fingerprint");
        }
    }
}
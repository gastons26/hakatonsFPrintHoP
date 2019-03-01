using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.DependencyInjection;

namespace HoP.FPrint.Gateway.AppStart
{
    public class StartUp
    {
        public void ConfigureServices(IServiceCollection services){
            services.AddMvc();
        }
        
        public void Configure(IApplicationBuilder app)
        {
            app.UseMvc();
            app.Run(async (context) => { await context.Response.WriteAsync("Hello finger print magnet"); });
        }
    }
}
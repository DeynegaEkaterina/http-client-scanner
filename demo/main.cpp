#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include "filesystem"


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;           
bool status = true;

int main(int argc, char** argv)
{
  try
  {
    if(argc != 2)
    {
      std::cerr <<
                "Usage: ./scanner path_to_directory \n" <<
                "Example:\n" <<
                "./scanner /home/ekaterina/CLionProjects/lab-05-stack\n";
      return EXIT_FAILURE;
    }
    auto const host = "0.0.0.0";
    auto const port = "8080";

    while(status) {
        net::io_context ioc;

    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    auto const results = resolver.resolve(host, port);

    stream.connect(results);
        std::filesystem::path p = argv[1];
      http::string_body::value_type body = p;

      http::request<http::string_body> req{http::verb::post, "/",11};
      req.set(http::field::host, host);
      req.body() = body;
      req.prepare_payload();
      http::write(stream, req);
      beast::flat_buffer buffer;
      http::response<http::string_body> res;
      http::read(stream, buffer, res);
      std::cout << res.body() << std::endl;
      std::cout << "Do u want to continue? Y/n" << std::endl;
      std::string continue_choice;
      std:: cin >> continue_choice;
      if ( continue_choice == "n" || continue_choice == "N"){
        status = false;
      }
      beast::error_code ec;
      stream.socket().shutdown(tcp::socket::shutdown_both, ec);
      if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};
    }

  }
  catch(std::exception const& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

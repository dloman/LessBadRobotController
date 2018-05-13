#include <Websockets/Client.hpp>

#include <SFML/Window.hpp>
#include <SFML/Window/Joystick.hpp>

#include <boost/program_options.hpp>

#include <iostream>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DoJoystick()
{
  sf::Joystick::update();

  std::cout <<
    std::boolalpha <<
    0 <<
    " is connected ? == " <<
    sf::Joystick::isConnected(0) <<
    std::endl;

  while (true)
  {
    sf::Joystick::update();

    std::cout << std::fixed << "Y = " << sf::Joystick::getAxisPosition(0, sf::Joystick::Y) << " ";
    std::cout << std::fixed << "X = " << sf::Joystick::getAxisPosition(0, sf::Joystick::X) << " ";
    std::cout << std::fixed << "U = " << sf::Joystick::getAxisPosition(0, sf::Joystick::U) << '\r';

    std::cout.flush();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    using namespace boost::program_options;

    std::string Hostname = "localhost";
    unsigned Port = 8181u;

		options_description Description("Usage");
    try
    {
      Description.add_options()
        ("help,h",     "produce help message")
        ("hostname,u", value<std::string>(&Hostname), "set the host server")
        ("port,p",   value<unsigned>(&Port), "set the server port");
    }
    catch (std::exception& Exception)
    {
      std::cout << Description << std::endl;

      return 1;
    }

    variables_map Arguments;

    store(parse_command_line(argc, argv, Description), Arguments);

    notify(Arguments);

    if (Arguments.count("help"))
    {
      std::cout << Description << std::endl;

      return 1;
    }

    auto pWebsocket = dl::ws::Client::Create(Hostname, Port);

    pWebsocket->GetOnRxSignal().Connect([] (auto Bytes)
      {
        std::cout << Bytes << std::endl;
      });

    pWebsocket->GetOnDisconnectSignal().Connect(
      []
      {
        std::cout << "server has disconnected" << std::endl;
      });

    pWebsocket->GetErrorSignal().Connect(
      [] (const std::string& Error)
      {
        std::cout << "ERROR: " << Error << std::endl;
      });

    //pWebsocket->Connect();

    DoJoystick();

    return 0;
}

/**
 *  @example example_project.cpp
 */

#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include <iostream>
#include <cstring>
#include "Cmd.h"

LibSerial::SerialStream serial_stream ;


void help(int arg_cnt, char**args)
{
  char commands[] = "Command List:\n"
  "version\n"
  "show-warranty\n"
  "get-config\n"
  "set-config\n"
  "set-slot-config\n"
  "clear-slot\n"
  "set-rtc\n"
  "get-rtc\n"
  "restart\n"
  "set-site-name\n"
  "set-deployment-identifier\n"
  "set-interval\n"
  "set-burst-number\n"
  "set-start-up-delay\n"
  "set-burst-delay\n"
  "calibrate\n"
  "set-user-note\n"
  "set-user-value\n"
  "start-logging\n"
  "deploy-now\n"
  "interactive\n"
  "trace\n"
  "check-memory\n"
  "scan-ic2\n";

  std::cout << commands << std::endl;
}

void relay(int arg_cnt, char **args)
{
    for(int i=0; i< arg_cnt; i++)
    {
        serial_stream << args[i];
    }
    serial_stream << std::endl;
}


int main()
{
    
    char port[50] = "/dev/ttyACM0";
    std::cout << "opening " << port << std::endl;

    serial_stream.Open( port ) ;
    std::cout << "opened" << std::endl;

    // Set the baud rates.
    using LibSerial::BaudRate ;
    // serial_port.SetBaudRate( BaudRate::BAUD_115200 ) ;
    serial_stream.SetBaudRate( BaudRate::BAUD_115200 ) ;

    // Create a few variables with data we can send.
    char write_byte_1 = 'a' ;
    char write_byte_2 = 'b' ;

    char read_byte_1 = 'A' ;
    char read_byte_2 = 'B' ;

    // Read a byte to the serial port using SerialPort Write() methods.
    // serial_port.WriteByte(write_byte_1) ;

    char buffer[200];

    std::cout << "connecting" << std::endl;

    cmdInit(&std::cin, &std::cout);
    cmdAdd("help", help);
    cmdAdd("relay", relay);


    // With SerialStream objects you can read/write to the port using iostream operators.
    while(true)
    {
        serial_stream.getline(buffer, 200, '\a');
        if(strlen(buffer) > 0)
        {
            std::cout << ".> " << buffer << std::endl ;
        }
        
        cmdPoll();

        // if(strcmp(buffer, "\u200B\u200B\u200B\u200B\r") == 0)
        // {
        //     std::cout << "will read" << std::endl;
        //     std::cin.getline(buffer, 200);
        //     std::cout << "did read" << std::endl;
        //     // if(strlen(wbuffer) > 0)
        //     // {
        //         std::cout << "done read" << std::endl;
        //         std::cout << buffer << std::endl;
        //     // }
        //     // else
        //     // {
        //         std::cout << "gotnothing" << std::endl;
        //     // }
        // }
   
    }
    serial_stream.Close() ;
}

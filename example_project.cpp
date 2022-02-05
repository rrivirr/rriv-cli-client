/**
 *  @example example_project.cpp
 */

#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include <iostream>
#include <cstring>

int main()
{
    using LibSerial::SerialPort ;
    using LibSerial::SerialStream ;

    // You can instantiate a Serial Port or a Serial Stream object, whichever you'd prefer to work with.
    // For this example, we will demonstrate by using both types of objects.
    SerialPort serial_port ;
    SerialStream serial_stream ;

    // Open hardware serial ports using the Open() method.
    // serial_port.Open( "/dev/ttyACM0" ) ;
    serial_stream.Open( "/dev/ttyACM0" ) ;

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

    // With SerialStream objects you can read/write to the port using iostream operators.
    while(strcmp(buffer, "CMD >> ") != 0)
    {
        serial_stream.getline(buffer, 100);
        if(strlen(buffer) > 0)
        {
            std::cout << "> " << buffer << std::endl ;
        }
    }
    std::cout << "> " << buffer << std::endl ;
    // std::cin.getline

    std::cout << "sending" << std::endl;
    // serial_stream << "help\r" << std::endl; ;

    // // Specify a timeout value (in milliseconds).
    // size_t timeout_milliseconds = 25 ;

    // using LibSerial::ReadTimeout ;
    // try
    // {
    //     // Read a byte from the serial port using SerialPort Read() methods.
    //     // serial_port.ReadByte(read_byte_1, timeout_milliseconds) ;

    //     // With SerialStream objects you can read/write to the port using iostream operators.
    //     std::cout << "getting" << std::endl;
    //     while(strcmp(buffer, "CMD> ") != 0)
    //     {
    //         serial_stream.getline(buffer, 100);
    //         std::cout << "> " << buffer << std::endl ;
    //     }
    //     std::cout << "> " << buffer << std::endl ;

    // }
    // catch (const ReadTimeout&)
    // {
    //     std::cerr << "The Read() call has timed out." << std::endl ;
    // }
    
    std::cout << "got" << std::endl;

    // std::cout << "serial_port read:   " << read_byte_1 << std::endl ;
    std::cout << "> " << buffer << std::endl ;

    // Close the Serial Port and Serial Stream.
    // serial_port.Close() ;
    serial_stream.Close() ;
}

/**
 *  @example example_project.cpp
 */

#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include <iostream>
#include <cstring>
#include "lib/Cmd.h"
#include <unistd.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <dirent.h>

using namespace std;


LibSerial::SerialPort serial_port ;
bool processingCommandFile = false;
bool processingTestFile = false;
std::ifstream commandFile;
std::string testResult;

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

void setRTC(int arg_cnt, char **args)
{
    std::time_t time = std::time(nullptr);
    std::cout << asctime(std::localtime(&time)) << time << std::endl;

    std::stringstream timeStringStream;
    timeStringStream << time;

    serial_port.Write("set-rtc " + timeStringStream.str() + "\r\n");
    serial_port.DrainWriteBuffer();
}

void runFile(int arg_cnt, char **args)
{
    processingCommandFile = true;
    commandFile = std::ifstream(args[1]);
}

void runTest(int arg_cnt, char **args)
{
    processingCommandFile = true;
    processingTestFile = true;
    commandFile = std::ifstream(args[1]);
}

void loadSlotFromFile(int arg_cnt, char **args)
{
    std::ifstream slotFile(args[1]);
    // read and remove all return characters
    std::string slotSettings;
    std::string line;
    while(std::getline(slotFile, line))
    {
        line.erase( std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase( std::remove(line.begin(), line.end(), '\n'), line.end());
        line.erase( std::remove(line.begin(), line.end(), ' '), line.end());
        slotSettings = slotSettings + line;
    }
    std::string command = "set-slot-config " + slotSettings;
    std::cout << command << std::endl;
    cmdRun(command.c_str());
    slotFile.close();
}

void loadConfigFromFile(int arg_cnt, char **args)
{
    std::ifstream configFile(args[1]);
    // read and remove all return characters
    std::string config;
    std::string line;
    while(std::getline(configFile, line))
    {
        line.erase( std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase( std::remove(line.begin(), line.end(), '\n'), line.end());
        line.erase( std::remove(line.begin(), line.end(), ' '), line.end());
        config = config + line;
    }
    std::string command = "set-config " + config;
    std::cout << command << std::endl;
    cmdRun(command.c_str());
    configFile.close();
}




void relay(int arg_cnt, char **args)
{
    for(int i=0; i< arg_cnt; i++)
    {
        serial_port.Write(args[i]);
        if( i < arg_cnt - 1)
        {
            serial_port.Write(" ");
        }
    }
    serial_port.Write("\r\n");

}


vector<string> list_dir(const char *path, const char *match)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    vector<string> list = vector<string>();
    if (dir == NULL)
    {
        return list;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (match == NULL)
        {
            list.push_back(entry->d_name);
        }
        else
        {
            regex str_expr(match);
            if (regex_match(entry->d_name, str_expr))
                list.push_back(entry->d_name);
        }

    }
    closedir(dir);
    return list;
}

vector<string> list_dir(const char *path)
{
    return list_dir(path, NULL);
}


int main()
{
    string port("/dev/*");
    bool deviceSelected = false;
    bool waitForDevice = false;

    while (!deviceSelected)
    {

        vector<string> list = list_dir("/dev", "ttyACM.*");
        int i = 1;
        for (const auto &value : list)
        {
            cout << i << ") " << value << "\n";
        }

        if (list.size() == 0)
        {
            if(!waitForDevice)
            {
                cout << "No RRIV devices found." << endl;
                cout << "Please plug in a RRIV device." << endl;
                waitForDevice = true;
            }
            sleep(1);
        }

        if (list.size() == 1)
        {
            port = (string("/dev/") + list.at(0)).c_str();
            cout << "Automatically connecting to /dev/" + list.at(0) << endl;
            deviceSelected = true;
        }

        if(list.size() > 1)
        {
            cout << "Please select port to connect to" << endl;
            int portIndex;
            cin >> portIndex;
            port = (string("/dev/") + list.at(portIndex)).c_str();
            cout << "Cnnecting to /dev/" + list.at(0) << endl;
            deviceSelected = true;
        }
    }

    std::cin.sync_with_stdio(false);    using namespace std;

    std::cout << "opening " << port << std::endl;

    serial_port.Open( port ) ;
    std::cout << "opened" << std::endl;

    // Set the baud rates.
    using namespace LibSerial;
    serial_port.SetBaudRate( BaudRate::BAUD_115200 );
    serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8);
    serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE);
    serial_port.SetParity(Parity::PARITY_NONE) ;
    serial_port.SetStopBits(StopBits::STOP_BITS_1) ;


    char buffer[200];

    std::cout << "connecting" << std::endl;

    cmdInit(&std::cin, &std::cout);
    cmdAdd("help", help);
    cmdAdd("set-rtc", setRTC);
    cmdAdd("run-file", runFile);
    cmdAdd("run-test", runTest);
    cmdAdd("load-config", loadConfigFromFile);
    cmdAdd("load-slot-config", loadSlotFromFile);
    cmdAdd("relay", relay);

    std::cout << "cmd ready" << std::endl;

    serial_port.Write("restart\r\n");
    serial_port.DrainWriteBuffer();

    
    bool interfaceReady = false;
    std::string readString;

    // wait for prompt
    while(!interfaceReady)
    {
        while (serial_port.IsDataAvailable())
        {

            try
            {
                serial_port.ReadLine(readString);
            }
            catch (const std::exception &)
            { /* */
            }

            if(readString == "CMD >> ")
            {
                // serial_port.Write("no-prompt\r\n"); // place into relay mode
                // serial_port.DrainWriteBuffer();

                interfaceReady = true;
            }

            if (readString.size() > 0)
            {
                std::cout << ">";
                std::cout << readString;
                std::cout.flush();
            }

   
        }
    }

    while(true)
    {
        while(serial_port.IsDataAvailable()){

            try 
            {
                serial_port.Read(readString, 200, 10);
            }
            catch (const std::exception&) { /* */ }

            if(readString.size() > 0)
            {
                // use a standard replace code to remove the prompt sent by the peripheral
                //  readString = std::replace(readString, std::string("CMD >> "), std::string("") );
                //readString.replace("CMD >> ", "");
                std::cout << readString;
                std::cout.flush();
            }   


            if(processingTestFile)
            {
                if(readString == testResult)
                {
                    std::cout << "Test Success!" << std::endl;
                }
                else
                {
                    std::cout << "Test Failed!" << std::endl;
                }
            } 

            usleep(1000);

        }

        if(processingCommandFile)
        {
            std::string line;
            if(std::getline(commandFile, line))
            {
                if(processingTestFile)
                {
                    std::stringstream check1(line);
                    std::string intermediate;
                    std::getline(check1, intermediate, ':');
                    std::string command = intermediate;
                    std::getline(check1, intermediate, ':');
                    testResult = intermediate;
                    cmdRun(command.c_str());
                }
                else
                {
                    // serial_port.Write(line + "\r\n"); // instead pass through Cmd.cpp somehow
                    cmdRun(line.c_str());   
                }
            }
            else
            {
                processingCommandFile = false;
                commandFile.close();
            }
        }
        else
        {
            cmdPoll();
        }

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
        usleep(100000);
    }
    serial_port.Close() ;
}

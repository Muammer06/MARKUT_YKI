#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <array>
#include <variant>
#include "commands.hpp"

#ifndef JETSON_COMMAND_HPP
#define JETSON_COMMAND_HPP



class Reciever{
    private:
        
        int client_socket;
        int port = 14001;
        sockaddr_in addr;
        std::string targetIP = "127.0.0.1";
    public:
        
        enum MessageFormat{
            STRCOMMAND = 0,
            MISSION = 1,
            NULLMESSAGE = 2
        };
        Reciever(char host[16] = "127.0.0.1"){
            this->targetIP = host;
            this->port = port;
            this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket == -1) {
                std::cerr << "Soket oluşturulamadı!" << std::endl;
            }

            this->addr.sin_family = AF_INET;
            this->addr.sin_port = htons(port);
            inet_pton(AF_INET, host, &this->addr.sin_addr);

            if (connect(this->client_socket, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1) {
                std::cerr << "Bağlantı hatası!" << std::endl;
                close(this->client_socket);
            }
        }
        ~Reciever(){
            close(this->client_socket);
        }
        class Message{
            public:
                MessageFormat messageFormat;
                std::string message;

                Message(MessageFormat messageFormat, std::string message){
                    this->messageFormat = messageFormat;
                    this->message = message;
                }
        };
        Message* receiveCommand() {
            char buffer[1024] = {0};  
            ssize_t recvBytes = recv(this->client_socket, buffer, sizeof(buffer) - 1, 0);

            if (recvBytes == -1) {
                std::cerr << "Mesaj alınamadı!" << std::endl;
                close(this->client_socket);
                return nullptr;
            }

            std::string messageStr(buffer, recvBytes);
            std::cout << "Mesaj alındı: " << messageStr << std::endl;

            MessageFormat messageFormat;

            if (messageStr.empty()) {
                std::cerr << "Mesaj boş!" << std::endl;
                messageFormat = NULLMESSAGE;
                return new Message(NULLMESSAGE,"Null");
            } else if (messageStr.find("{") != std::string::npos) {
                messageFormat = MISSION;
            } else {
                messageFormat = STRCOMMAND;
            }

            return new Message(messageFormat, messageStr);
        }

        using FlexibleMessageType = std::variant<std::string, Command*>;
        FlexibleMessageType parseCommand(Message* message){  
            if(message->message != "Null"){
            std::vector<std::string> result;
            std::stringstream ss(message->message);
            std::string item;          
            if(message->messageFormat == STRCOMMAND){
                while (getline(ss, item, ',')) {
                    result.push_back(item);
                }
                Command* command = new Command();
                command->messageID = std::stoi(result[0]);
                command->param1 = result[1];
                command->param2 = result[2];
                command->param3 = result[3];
                command->message = message->message;
                return command;
            }else if(message->messageFormat == MISSION){
                return message->message;
            }
            }else{
                return "Null";
            }
        }

};

#endif
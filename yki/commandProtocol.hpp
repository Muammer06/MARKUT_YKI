#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "commands.hpp"

#ifndef YKI_COMMANDPROTOCOL_HPP
#define YKI_COMMANDPROTOCOL_HPP


class CommandManager {
    public:
        enum CommandID{
            HEARTBEAT = 0
        };

        CommandManager(){
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock == -1) {
            throw std::runtime_error("UDP socket oluşturulamadı");
            }
            memset(&this->addr, 0, sizeof(this->addr));
            this->addr.sin_family = AF_INET;
            this->addr.sin_port = htons(this->port);
            this->addr.sin_addr.s_addr = INADDR_ANY;

            if (bind(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) == -1) {
            close(this->sock);
            throw std::runtime_error("Socket bind hatası");
            }

            if (listen(this->sock, 5) == -1) {
            close(this->sock);
            throw std::runtime_error("Socket listen hatası");
            }

            std::cout << "Sunucu başlatıldı. Bağlantı bekleniyor..." << std::endl;

            sockaddr_in client_addr{};
            socklen_t client_addr_len = sizeof(client_addr);
            this->client_socket = accept(this->sock, (struct sockaddr*)&client_addr, &client_addr_len);
            if (this->client_socket == -1) {
            std::cerr << "Bağlantı kabul edilemedi!" << std::endl;
            close(this->sock);
            }

            std::cout << "Bağlantı kuruldu." << std::endl;
        }
        ~CommandManager(){
            close(this->client_socket);
            close(this->sock);
        }
        void sendMessage(Command* command) {
            std::cout << "Sending message: " << command->commandName << std::endl;
            std::string message = command->message;
            std::cout<<message<<std::endl; 
            size_t totalSent = 0;
            ssize_t sendBytes;
            
            while (totalSent < message.size()) {
                sendBytes = send(this->client_socket, message.c_str() + totalSent, message.size() - totalSent, 0);
                if (sendBytes == -1) {
                    std::cerr << "Mesaj gönderilemedi!" << std::endl;
                    return;
                }
                totalSent += sendBytes;
            }

            std::cout << "Mesaj gönderildi: " << totalSent << " bytes" << std::endl;
        }


        void receiveMessage(){
            uint16_t messageID;
            int recvBytes = recv(this->client_socket, &messageID, sizeof(messageID), 0);
            if (recvBytes == -1) {
                std::cerr << "Mesaj alınamadı!" << std::endl;
                close(this->sock);
            }
            std::cout << "Mesaj alındı: " << messageID << std::endl;
        }

        void setPort(int port){
            this->port = port;
        }
        int getPort(){
            return this->port;
        }
    private:
        int sock;
        sockaddr_in addr;
        int client_socket;
        int port = 14001;


};





#endif
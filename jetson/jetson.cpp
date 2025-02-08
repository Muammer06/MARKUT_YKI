#include "command.hpp"
#include "commands.hpp"
#include "mission.hpp"
#include <any>
#include <variant>
#include <iomanip>

int main(){
    Reciever reciever("127.0.0.1");
    MissionManager* missionManager = new MissionManager(&reciever);
    while (true)
    {
        try
        {
            Reciever::Message* message = reciever.receiveCommand();
            using FlexibleMessageType = std::variant<std::string, Command*>;
            FlexibleMessageType parsedCommand = reciever.parseCommand(message);
            //std::cout<<message->messageFormat<<std::endl;
            if(message->messageFormat == Reciever::MessageFormat::NULLMESSAGE){
                delete message;
                continue;
            }else{
                if (message->messageFormat == Reciever::MessageFormat::STRCOMMAND)
                {
                    Command* command = std::get<Command*>(parsedCommand);
                    //std::cout<<"id :"<<command->messageID<<std::endl;
                    std::cout<<command->param1<<std::endl;
                    switch (command->messageID)
                    {
                    case PARAM_SET:
                        std::cout<<command->param2<<std::setprecision(9)<<std::endl;
                        break;
                    
                    default:
                        break;
                    }
                    delete command;
                    delete message;

                }else if(message->messageFormat == Reciever::MessageFormat::MISSION){
                    std::string str = std::get<std::string>(parsedCommand);
    
                    missionManager->parseMissionJsonItems(str);
                    for(auto item : missionManager->MissionItemList){
                        std::cout<<item->lat<<std::endl;
                    }
                    delete message;
    
                }else{
                    delete message;
                    continue;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            break;
        }
    }
    delete missionManager;
    return 0;

}
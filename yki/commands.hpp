#include <iostream>

#ifndef YKI_COMMANDS_HPP
#define YKI_COMMANDS_HPP

class Command{
    public:
        uint16_t messageID;
        std::string commandName = "";
        std::string param1 = "";
        std::string param2 = "";
        std::string param3 = "";
        std::string message = this->messageID + "," + this->param1 + "," + this->param2 + "," + this->param3;

    private:
        
};

class Heartbeat: public Command{
    private:

    public:
        Heartbeat(){
            this->messageID = 0;
            this->commandName = "HEARTBEAT";
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class SetMode: public Command{
    private:

    public:
        SetMode(u_int8_t mode){
            this->messageID = 1;
            this->commandName = "SET_MODE";
            this->param1 = std::to_string(mode);
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class Arm: public Command{
    private:

    public:        
        Arm(bool arm){
            this->messageID = 2;
            this->commandName = "ARM";
            this->param1 = arm ? "1" : "0";
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class MissionStart: public Command{
    private:

    public:
        uint16_t messageID = 3;
        
        MissionStart(){
            this->commandName = "MISSION_START";
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class MissionClear: public Command{
    private:

    public:
        uint16_t messageID = 4;
        
        MissionClear(){
            this->commandName = "MISSION_CLEAR";
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class RTL: public Command{
    private:

    public:
        uint16_t messageID = 5;
        
        RTL(){
            this->commandName = "RTL";
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class RTLMode: public Command{
    private:

    public:
        uint16_t messageID = 6;
        
        RTLMode(u_int8_t mode){
            this->commandName = "RTL_MODE";
            this->param1 = std::to_string(mode);
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

class ParamSet: public Command{
    private:

    public:
        uint16_t messageID = 7;
        
        ParamSet(std::string paramName, float paramValue){
            this->commandName = "PARAM_SET";
            this->param1 = paramName;
            this->param2 = std::to_string(paramValue);
            this->message = std::to_string(this->messageID) + "," + this->param1 + "," + this->param2 + "," + this->param3;
        }
};

enum CommandList{
    HEARTBEAT,
    SET_MODE,
    ARM,
    MISSION_START,
    MISSION_CLEAR,
    RTL,
    RTL_MODE,
    PARAM_SET
};

#endif
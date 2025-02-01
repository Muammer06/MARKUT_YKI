#include <iostream>
#include "commandProtocol.hpp"
#include "commands.hpp"

int main(){
    CommandManager manager;
    Command* command = new ParamSet("param",3.0f);
    manager.sendMessage(command);
    usleep(10000);
    manager.sendMessage(new Heartbeat());
    usleep(10000);
    manager.sendMessage(new Heartbeat());
    return 0;
}
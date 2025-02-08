#include <iostream>
#include "commandProtocol.hpp"
#include "commands.hpp"
#include "mission.hpp"

int main(){
    CommandManager manager;
    Command* command = new ParamSet("param",3.0f);
    manager.sendMessage(command);
    usleep(10000);
    manager.sendMessage(new Heartbeat());
    usleep(10000);
    manager.sendMessage(new Heartbeat());
    usleep(10000);
    Mission* mission = new Mission();
    mission->addMissionItem(32.6957302f,22.29304231f,3);
    mission->addMissionItem(32.845760432f,22.29875390f,6);
    mission->addMissionItem(32.545239843f,22.4542397402f,9);
    manager.sendMessage(nullptr,mission->getMissionItemsAsJsonString());
    return 0;
}
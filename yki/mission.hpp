#include <iostream>
#include <vector>
#include <sstream>
#include "commandProtocol.hpp"
#include <string>
#include <iomanip>

#ifndef YKI_MISSION_HPP
#define YKI_MISSION_HPP

struct MissionItem
{
    int queue =0;
    float lat;
    float lng;
    int alt;
};

class Mission{
    private:
        std::vector<MissionItem*> MissionItemList;
    public:
        Mission(){
        }
        ~Mission(){
            for (int i = 0; i < this->MissionItemList.size(); i++)
            {
                delete this->MissionItemList[i];
            }
        }
        void addMissionItem(float lat,float lng,int alt){
            MissionItem* item = new MissionItem();
            item->queue = this->MissionItemList.size();
            item->lat = lat;
            item->lng = lng;
            item->alt = alt;
            this->MissionItemList.push_back(item);
        }
        void removeMissionItem(int queue){
            for (int i = 0; i < this->MissionItemList.size(); i++)
            {
                if(this->MissionItemList[i]->queue == queue){
                    delete this->MissionItemList[i];
                    this->MissionItemList.erase(this->MissionItemList.begin()+i);
                    break;
                }
            }
        }
        void clearMission(){
            for (int i = 0; i < this->MissionItemList.size(); i++)
            {
                delete this->MissionItemList[i];
            }
            this->MissionItemList.clear();
        }
        std::vector<MissionItem*> getMissionItems(){
            return this->MissionItemList;
        }
        std::string getMissionItemsAsJsonString(){
            std::ostringstream json;
            json << std::fixed << std::setprecision(9);
            json << "{Mission:{";
            
            for (size_t i = 0; i < MissionItemList.size(); ++i) {
                json << "Item{lat:" << MissionItemList[i]->lat
                    << ",lng:" << MissionItemList[i]->lng
                    << ",alt:" << MissionItemList[i]->alt << "}";
                if (i != MissionItemList.size() - 1) {
                    json << ",";
                }
            }
            
            json << "}}";
            return json.str();
        }
        void sendMissionItemsToDrone(CommandManager* commandManager){
            std::cout << "Sending Mission Items to Drone" << std::endl;
            commandManager->sendMessage(nullptr,this->getMissionItemsAsJsonString());
        }
};


#endif
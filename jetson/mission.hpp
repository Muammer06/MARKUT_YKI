#include <iostream>
#include <vector>
#include "command.hpp"
#include <algorithm>
#include <cctype>

#ifndef JETSON_MISSION_HPP
#define JETSON_MISSION_HPP

struct MissionItem
{
    int queue =0;
    float lat=0.0f;
    float lng=0.0f;
    int alt=0;
};

class MissionManager{
    private:
        Reciever* reciever;
        

    public:
        std::vector<MissionItem*> MissionItemList;
        MissionManager(Reciever* reciever){
            this->reciever = reciever;
        }
        void parseMissionJsonItems(std::string jsonString) {
            std::string::size_type pos = 0;
            std::cout << jsonString << std::endl;

            while ((pos = jsonString.find("Item{", pos)) != std::string::npos) {
                auto endPos = jsonString.find("}", pos);
                if (endPos == std::string::npos) break;

                std::string itemString = jsonString.substr(pos + 5, endPos - pos - 5); // Skip "Item{"
                std::istringstream itemStream(itemString);
                std::string keyValue;

                MissionItem* missionItem = new MissionItem();

                while (std::getline(itemStream, keyValue, ',')) {
                    auto colonPos = keyValue.find(":");
                    if (colonPos == std::string::npos) continue;

                    std::string key = keyValue.substr(0, colonPos);
                    std::string value = keyValue.substr(colonPos + 1);

                    // Remove whitespace
                    key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());
                    value.erase(std::remove_if(value.begin(), value.end(), isspace), value.end());

                    if (key == "lat") {
                        missionItem->lat = std::stof(value);
                    } else if (key == "lng") {
                        missionItem->lng = std::stof(value);
                    } else if (key == "alt") {
                        missionItem->alt = std::stoi(value);
                    }
                }

                MissionItemList.push_back(missionItem);
                pos = endPos + 1;
            }
        }
};

#endif
#pragma once
// The following is under maintenance
// #include "json.hpp"
// #include <fstream>

// class Utilis
// {
// public:
//     static constexpr int intForLowPriority = 1;
//     static constexpr int intForMediumPriority = 2;
//     static constexpr int intForHighPriority = 3;

//     static constexpr int intForToDoStatus = 1;
//     static constexpr int intForInProgressStatus = 2;
//     static constexpr int intForCompletedStatus = 3;

// public:
//     static void saveConfig()
//     {
//         std::ofstream write;
//         write.open("config.json");

//         nlohmann::json Cjson;

//         Cjson["priorities"]["low"] = intForLowPriority;
//         Cjson["priorities"]["medium"] = intForMediumPriority;
//         Cjson["priorities"]["high"] = intForHighPriority;

//         Cjson["statuses"]["todo"] = intForToDoStatus;
//         Cjson["statuses"]["inprogress"] = intForInProgressStatus;
//         Cjson["statuses"]["completed"] = intForCompletedStatus;

//         write << Cjson.dump(4);
//         write.close();
//     }

//     static void loadConfig()
//     {
//         std::ifstream read;
//         read.open("config.json");

//         if (read.is_open()) {
//             nlohmann::json Cjson;
//             read >> Cjson;

//             if (Cjson.contains("priorities")) {
//                 intForLowPriority = Cjson["priorities"]["low"];
//                 intForMediumPriority = Cjson["priorities"]["medium"];
//                 intForHighPriority = Cjson["priorities"]["high"];
//             }

//             if (Cjson.contains("statuses")) {
//                 intForToDoStatus = Cjson["statuses"]["todo"];
//                 intForInProgressStatus = Cjson["statuses"]["inprogress"];
//                 intForCompletedStatus = Cjson["statuses"]["completed"];
//             }

//             read.close();
//         }
//     }
// };

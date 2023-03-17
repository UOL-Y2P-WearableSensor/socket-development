//
// Created by 沈逸潇 on 19/02/2023.
//

#ifndef WEARABLESENSOR_MACOS_IMU_JSON_H
#define WEARABLESENSOR_MACOS_IMU_JSON_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#define FRAME2BROWSER 1
#define IMU_NUM 5
const std::string name[IMU_NUM]={"L_F","R_F","L_T", "R_T", "OnBrd"};
struct Euler_angle_t {
    std::string data[2];
//    std::string data[3];
    std::string toString() const;
};

struct IMU_Data_t {
    std::vector<Euler_angle_t> data[IMU_NUM];
    std::string toString() const;
};

std::string send_IMU_data();



#endif //WEARABLESENSOR_MACOS_IMU_JSON_H

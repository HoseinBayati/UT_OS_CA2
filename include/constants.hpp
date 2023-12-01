#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <vector>
#include <string>

constexpr int BUFFER_CAPACITY = 512;
constexpr char BILLS_PIPE[] = "BILLS_PIPE";
// constexpr int PIPE_WAIT_TIME = 1000000;

const std::string RESOURCES[3] = {"Gas", "Water", "Electricity"};
const char RESOURCE_SIGNS[3] = {'G', 'W', 'E'};
constexpr int RESOURCES_COUNT = 3;

const std::string BILLS_FILE = "bills.csv";

const std::string GAS = "Gas";
const std::string WATER = "Water";
const std::string ELECTRICITY = "Electricity";
const std::string GAS_CODE = "G";
const std::string WATER_CODE = "W";
const std::string ELECTRICITY_CODE = "E";
const std::string GAS_FILE = "Gas.csv";
const std::string WATER_FILE = "Water.csv";
const std::string ELECTRICITY_FILE = "Electricity.csv";

constexpr int MONTHS_COUNT = 12;
constexpr int HOURS_COUNT = 6;
constexpr int DAYS_COUNT = 30;

#endif
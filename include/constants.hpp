#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <vector>
#include <string>

constexpr int MAX_LEN = 512;
constexpr char NAMED_PIPE[] = "NAMED_PIPE";
constexpr int SLEEP_TIME = 0.5 * 1000 * 1000;

const std::string RESOURCES[3] = {"Gas", "Water", "Electricity"};
const char RESOURCE_SIGNS[3] = {'G', 'W', 'E'};
constexpr int RESOURCES_COUNT = 3;

const std::string GAS_FILE = "Gas.csv";
const std::string WATER_FILE = "Water.csv";
const std::string ELECTRICITY_FILE = "Electricity.csv";


constexpr int MONTHS_COUNT = 12;
constexpr int HOURS_COUNT = 6;
constexpr int DAYS_COUNT = 30;

#endif
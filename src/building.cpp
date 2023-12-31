#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "../include/util.hpp"
#include "../include/constants.hpp"

using namespace std;

void print_building_header(string building_name)
{
    cout << endl
         << endl
         << "=======================================" << endl
         << "Information for building: " << building_name << endl;
}

void calc_sum_for_each_month(
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT],
    int (&result)[MONTHS_COUNT])
{
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        result[i] = 0;
    }

    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        for (int j = 0; j < DAYS_COUNT; j++)
        {
            for (int k = 0; k < HOURS_COUNT; k++)
            {
                result[i] += consumption_info[i][j][k];
            }
        }
    }
}

int calc_mean_in_month(int sum_for_each_month[MONTHS_COUNT])
{
    int year_sum = 0;

    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        year_sum += sum_for_each_month[i];
    }

    int mean = year_sum / MONTHS_COUNT;

    return mean;
}

void calc_peak_in_each_month(
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT],
    int (&peak_hours)[MONTHS_COUNT])
{
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        int max_consumption = 0;
        for (int k = 0; k < HOURS_COUNT; k++)
        {
            int this_hour_consumption = 0;
            for (int j = 0; j < DAYS_COUNT; j++)
            {
                this_hour_consumption += consumption_info[i][j][k];
            }
            if (this_hour_consumption > max_consumption)
            {
                max_consumption = this_hour_consumption;
                peak_hours[i] = k;
            }
        }
    }
}

void calc_bill_for_each_month(
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT],
    int (&prices)[MONTHS_COUNT][RESOURCES_COUNT],
    int (&peak_hours)[MONTHS_COUNT],
    int mean_in_month,
    int (&result)[MONTHS_COUNT],
    const char *resource_code)
{
    int resource_col;

    if (resource_code == GAS_CODE)
        resource_col = GAS_COL;
    if (resource_code == WATER_CODE)
        resource_col = WATER_COL;
    if (resource_code == ELECTRICITY_CODE)
        resource_col = ELECTRICITY_COL;

    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        result[i] = 0;
        for (int j = 0; j < DAYS_COUNT; j++)
        {
            for (int k = 0; k < HOURS_COUNT; k++)
            {
                int this_hour_cost = (consumption_info[i][j][k] * prices[MONTHS_COUNT][resource_col]);

                if (k == peak_hours[i])
                {
                    int peak_amplitude = 1;
                    if (resource_code == WATER_CODE)
                    {
                        peak_amplitude = WATER_PENALTY;
                    }
                    if (resource_code == ELECTRICITY_CODE)
                    {
                        peak_amplitude = consumption_info[i][j][k] >= (mean_in_month / (DAYS_COUNT * HOURS_COUNT)) ? ELECTRICITY_PENALTY : ELECTRICITY_REWARD;
                    }
                    if (resource_code == GAS_CODE)
                    {
                        peak_amplitude = GAS_PENALTY;
                    }

                    this_hour_cost *= peak_amplitude;
                }

                result[i] += this_hour_cost;
            }
        }
    }
}

int calc_mean_peak_difference(
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT],
    int (&peak_hours)[MONTHS_COUNT],
    int (&sum_for_each_month)[MONTHS_COUNT])
{
    int total_year_sum = 0;
    int peak_consumption_year_sum = 0;
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        int peak_consumption_month_sum = 0;
        for (int j = 0; j < DAYS_COUNT; j++)
        {
            peak_consumption_month_sum += consumption_info[i][j][peak_hours[i]];
        }
        peak_consumption_year_sum += peak_consumption_month_sum;
    }

    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        total_year_sum += sum_for_each_month[i];
    }

    return ((peak_consumption_year_sum * HOURS_COUNT) - total_year_sum) / (MONTHS_COUNT * DAYS_COUNT * HOURS_COUNT);
}

void read_file(
    string file_dir,
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT])
{
    fstream fin;
    string line;
    vector<string> words;
    int hour_0_column = 3;

    fin.open(file_dir, ios::in);
    if (!fin.is_open())
    {
        cout << "!! couldn't open the file (invalid building name)" << endl;
        return;
    }

    getline(fin, line);
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        for (int j = 0; j < DAYS_COUNT; j++)
        {
            getline(fin, line);
            words = split_string(line.c_str(), ',');
            for (int k = 0; k < HOURS_COUNT; k++)
            {
                consumption_info[i][j][k] = atoi(words[hour_0_column + k].c_str());
            }
        }
    }

    fin.close();
}

void print_building_results(
    string resource_name,
    int sum_for_each_month[MONTHS_COUNT],
    int peak_hours[MONTHS_COUNT],
    int bill_for_each_month[MONTHS_COUNT],
    int mean_in_month,
    int peak_mean_difference)
{
    cout << "Resource:  " << resource_name << endl
         << " -- Total consumption for each month :: ";
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        cout << sum_for_each_month[i] << " ";
    }
    cout << endl
         << " -- Bill for each month ::: ";
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        cout << bill_for_each_month[i] << " ";
    }
    cout << endl
         << " -- Mean consumption in month ::: " << mean_in_month;

    cout << endl
         << " -- Peak hour in each month ::: ";
    for (int i = 0; i < MONTHS_COUNT; i++)
    {
        cout << peak_hours[i] << " ";
    }
    cout << endl
         << " -- The difference between a peak hour and mean ::: " << peak_mean_difference << endl
         << "---------------------------------------" << endl;
}

void show_resource_info(
    int (&consumption_info)[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT],
    int (&prices)[MONTHS_COUNT][RESOURCES_COUNT],
    string resource_code)
{
    int sum_for_each_month[MONTHS_COUNT];
    int peak_hours[MONTHS_COUNT];
    int bill_for_each_month[MONTHS_COUNT];
    int mean_in_month;
    int peak_mean_difference;

    calc_sum_for_each_month(
        consumption_info,
        sum_for_each_month);

    mean_in_month = calc_mean_in_month(sum_for_each_month);

    calc_peak_in_each_month(
        consumption_info,
        peak_hours);

    calc_bill_for_each_month(
        consumption_info,
        prices,
        peak_hours,
        mean_in_month,
        bill_for_each_month,
        resource_code.c_str());

    peak_mean_difference = calc_mean_peak_difference(
        consumption_info,
        peak_hours,
        sum_for_each_month);

    print_building_results(
        resource_code,
        sum_for_each_month,
        peak_hours,
        bill_for_each_month,
        mean_in_month,
        peak_mean_difference);
}

void get_prices_from_bills_proc(
    int (&prices)[MONTHS_COUNT][RESOURCES_COUNT])
{
    int pipe = open(BILLS_PIPE, O_WRONLY);
    if (pipe == -1)
    {
        cerr << "Failed to open named pipe for writing" << endl;
        return;
    }

    char requestMessage[] = "Get Resource Prices";
    write(pipe, requestMessage, sizeof(requestMessage));
    close(pipe);

    pipe = open(BILLS_PIPE, O_RDONLY);
    if (pipe == -1)
    {
        cerr << "Failed to open named pipe for reading response" << endl;
        return;
    }

    usleep(PIPE_WAIT_TIME);

    char buffer[BUFFER_CAPACITY];
    int bytesRead = read(pipe, buffer, BUFFER_CAPACITY);

    if (bytesRead > 0)
    {
        vector<string> all_prices_list = split_string(buffer, ',');
        for (int i = 0; i < MONTHS_COUNT; i++)
        {
            vector<string> resource_prices = split_string(all_prices_list[i].c_str(), ' ');
            for (int j = 0; j < RESOURCES_COUNT; j++)
                prices[i][j] = atoi(resource_prices[j].c_str());
        }
    }
    else
    {
        cerr << "Failed to read response from named pipe" << endl;
    }

    close(pipe);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        perror("!! Bad arguments for building proc\n");
        exit(EXIT_FAILURE);
    }

    int write_fd = atoi(argv[1]);
    int read_fd = atoi(argv[2]);
    string building_dir = argv[3];
    string building_name = argv[4];

    char buffer[BUFFER_CAPACITY];
    read(read_fd, buffer, BUFFER_CAPACITY);

    vector<string> target_resources = split_string(buffer, ' ');

    int g_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];
    int w_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];
    int e_consumption[MONTHS_COUNT][DAYS_COUNT][HOURS_COUNT];

    print_building_header(building_name);

    int prices[MONTHS_COUNT][RESOURCES_COUNT];

    get_prices_from_bills_proc(prices);

    // for (int i = 0; i < MONTHS_COUNT; i++)
    // {
    //     for (int j = 0; j < RESOURCES_COUNT; j++)
    //     {
    //         cout << prices[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    for (auto &resource_key : target_resources)
    {
        if (resource_key == GAS_CODE)
        {

            string file_dir = building_dir + GAS_FILE;
            read_file(file_dir, g_consumption);
            show_resource_info(g_consumption, prices, GAS_CODE);
        }
        else if (resource_key == WATER_CODE)
        {
            string file_dir = building_dir + WATER_FILE;
            read_file(file_dir, w_consumption);
            show_resource_info(w_consumption, prices, WATER_CODE);
        }
        else if (resource_key == ELECTRICITY_CODE)
        {
            string file_dir = building_dir + ELECTRICITY_FILE;
            read_file(file_dir, e_consumption);
            show_resource_info(e_consumption, prices, ELECTRICITY_CODE);
        }
        else
        {
            cerr << "!! invalid resource key" << endl;
        }
    }

    write(
        write_fd,
        buffer,
        sizeof(buffer));
}

// Part A: This is an extension task that requires you to decode sensor data from CAN log files.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the signal definitions in SteeringBench.dbc, read each CAN capture in data/
// and turn it into a CSV with one row per decoded frame:
// t,u_commanded,y_measured
// eg:
// 0,15.0,0.0
// 0.005,15.0,0.0
// ...
// where t is the frame timestamp minus the first kept frame's timestamp (s), u_commanded is
// the decoded CmdAngularRate (deg/s), and y_measured is the decoded MeasuredAngle (deg).
// The above values are not real numbers; they are only there to show the expected data output format.
// Do this for all three captures:
// data/step_test.log       ->  data/step_test.csv
// data/reversal_test.log   ->  data/reversal_test.csv
// data/deadband_test.log   ->  data/deadband_test.csv
//
// The Row type, writeCsv(), and main() below are provided -- they loop the three logs, call your
// decodeLog(), and write the CSV in exactly the format above. You just need to implement decodeLog().
//
// You do not need to use any external libraries. Use the resources below to understand how to
// extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-fi  le-database-intro
//
// Sanity check: plot your CSVs (python3 plot_data.py) and compare against the pre-plotted
// data/*.png files -- they should match.
//
// Build & run (from the TA/ folder):
//     c++ -std=c++17`Question-A.cc -o decode
//     ./decode

#include <cstdio>
#include <fstream>  
#include <string>
#include <vector>
#include <cstdint>

// One output row.
struct Row {
    double t;            // seconds since the first kept frame
    double u_commanded;  // deg/s
    double y_measured;   // deg
};

// Read the candump log at `path` and return one Row per STEER_ActuatorLog frame, in order.
// Push one Row{t, u_commanded, y_measured} per kept frame.
std::vector<Row> decodeLog(const std::string& path) {
    std::vector<Row> rows;

    // TODO: your code here
    std::ifstream file(path);

    double times_start = -1.0;

    std::string datastorage;
    while (std::getline(file,datastorage)){

        size_t ID_Position = datastorage.find(" 200#");

        if (ID_Position == std::string::npos){
            
            continue; 
        }
        
        size_t Timestamp_start = datastorage.find('(');
        size_t Timestamp_end = datastorage.find(')');
        std::string timestamp_log = datastorage.substr(Timestamp_start+1, Timestamp_end-Timestamp_start-1);
        double timestamp_raw = std::stod(timestamp_log);

        std::string hex_data = datastorage.substr(ID_Position+5);


        if (times_start<0){

            times_start=timestamp_raw;

        }

        double relative_time = timestamp_raw - times_start;

        std::string y_hexpairs = hex_data.substr(2,2) + hex_data.substr(0,2);
        int16_t y_raw = static_cast<int16_t>(std::stoi(y_hexpairs,nullptr,16));
        double y_measured = y_raw*0.1;

        std::string u_hexpairs = hex_data.substr(6,2) + hex_data.substr(4,2);
        int16_t u_raw = static_cast<int16_t>(std::stoi(u_hexpairs,nullptr,16));
        double u_commanded = u_raw*0.1;

        rows.push_back({relative_time,u_commanded,y_measured});

    }

    return rows;
}

// Provided -- writes the rows to a CSV in the required format. Do not change.
void writeCsv(const std::string& path, const std::vector<Row>& rows) {
    std::ofstream f(path);
    f << "t,u_commanded,y_measured\n";
    for (const Row& r : rows)
        f << r.t << "," << r.u_commanded << "," << r.y_measured << "\n";
}

// Provided -- runs decodeLog() + writeCsv() for each of the three captures.
int main() {
    const char* names[] = {"step_test", "reversal_test", "deadband_test"};
    for (const char* n : names) {
        const std::string in  = std::string("data/") + n + ".log";
        const std::string out = std::string("data/") + n + ".csv";
        const std::vector<Row> rows = decodeLog(in);
        writeCsv(out, rows);
        std::printf("%-14s %6zu frames -> %s\n", n, rows.size(), out.c_str());
    }
    return 0;
}

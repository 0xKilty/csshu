#include <cpr/cpr.h>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <fstream>

using string = std::string;

void usage() {
    std::cout << "csshu: a tool to connect automatically to the fastest machine on the CSU network\n"
    << "Options:\n" 
    << "-u <username> specify a username\n"
    << "-f <file>     select a file for upload\n"
    << "-h            show help message\n"
    << "-q <quantity> the number of machines in the list\n"
    << "-l[tcamg]     list machines by specification\n"
    << "-[tcamg]      connect automatically to a machine by the specification\n"
    << "(t) uptime (c) CPU (a) Load Average\n"
    << "(m) Memory (g) GPU (default) Load Average\n";
    exit(0); 
}

string get_data(string &url) {
    string data = cpr::Get(cpr::Url{url}).text;
    data.erase(std::remove_if(data.begin(), data.end(), ::isspace),data.end());
    return data;
}

void iter_finds(string &data, size_t &start, size_t &end, string start_key, string end_key) {
    start = data.find(start_key, start+1);
    end = data.find(end_key, end+1);
}

void print_headers(int padding) {
    string colums[4] = {"CPU", "Load", "Memory", "GPU"};
    std::cout << "\e[1m";
    for (int i = 0; i < padding; i++) {
        std::cout << " ";
    }
    std::cout << std::setw(15) << "Hostname";
    for (int i = 0; i < sizeof(colums)/sizeof(colums[0]); i++) {
        std::cout << std::setw(9) << colums[i];   
    }
    std::cout << std::setw(12) << "Uptime\n";
    std::cout << "\e[0m";
}

void ssh_command(string &username, string &machine, string &file) {
    string host = username + "@" + machine + ".cs.colostate.edu";
    if (!file.empty()) {
        string path; 
        std::cout << "Path for " + file + " on " << machine << ": ";
        std::cin >> path;
        std::cout << "Executing: scp " << file << " " << host << ":" << path << "\n";
        system(("scp " + file + " " + host + ":" + path).c_str());
    }
    std::cout << "Executing: ssh " << host << "\n";
    system(("ssh " + host).c_str());
    exit(0);
}

int main(int argc, char** argv) {
    int c;
    int highlighted = 2;
    bool automatic = true;
    char mode;
    int quant = 5;
    std::string username;
    string file;
    while ((c = getopt(argc, argv, "f:l:q:u:cmtga")) != -1) {
        switch (c) {
        case 'l':
            mode = *optarg;
            automatic = false;
            break;
        case 'q':
            quant = atoi(optarg);
            break;
        case 'u':
            username = optarg;
            break;
        case 'f':
            file = optarg;
            break;
        case 'c':
            mode = 'c';
            break;
        case 'm':
            mode = 'm';
            break;
        case 'g':
            mode = 'g';
            break;
        case 't':
            mode = 't';
            break;
        case 'a':
            mode = 'a';
            break;
        case 'h':
            usage();
            break;
        default:
            usage();
            break;
        }
    }
    if (!file.empty()) {
        std::ifstream exist_check;
        exist_check.open(file);
        if (!exist_check) {
            std::cerr << argv[0] << " File: " << file << " does not exist\n";
            exit(1);
        }
    }
    if (username.empty()) {
        std::cout << "Username: ";
        std::cin >> username;
    }
    std::string url = "https://www.cs.colostate.edu/machinestats/";
    if (mode == 'c') {
        url +=  "?column=cpu&order=asc"; 
        highlighted = 1;
    } else if (mode == 'm') {
        url += "?column=percent_used_memory&order=asc";
        highlighted = 3;
    } else if (mode == 't') {
        url += "?column=uptime&order=asc";
        highlighted = 8;
    } else if (mode == 'g') {
        url += "?column=percent_used_gpu_memory&order=asc";
        highlighted = 4;
    } else if (mode == 'a') {
        url += "?column=load_avg&order=asc";
        highlighted = 2;
    } else {
        std::cout << "Connecting automatically...\n";
    }
    string data = get_data(url);
    std::size_t start = data.find("<tr>");
    std::size_t end = data.find("</tr>", start+1);
    iter_finds(data, start, end, "<tr>", "</tr>");
    int max = 8;
    string names[quant];
    int namesIndex = 0;
    int padding = std::to_string(quant).length() + 1;
    int displacement = 1;
    if (!automatic) {
        print_headers(padding);
    }
    for (int i = 0; i < quant; i++) {
        string machine = data.substr(start, end-start); 
        string key = "<td>";
        std::size_t start_tag = machine.find(key);
        std::size_t end_tag = machine.find("</td>");
        string name = machine.substr(start_tag+key.length(), end_tag-start_tag-key.length());
        iter_finds(machine, start_tag, end_tag, key, "</td>");
        string location = machine.substr(start_tag+key.length(), end_tag-start_tag-key.length());
        if (location != "Retired") {
            names[namesIndex] = name;
            namesIndex++;
            if (namesIndex == 1 && automatic) {
                ssh_command(username, name, file);
            }
            string tag = std::to_string(i+displacement) + ")";
            if (!automatic) {
                std::cout << tag;
            }
            for (int j = 0; j < padding-tag.length(); j++) {
                std::cout << " ";
            }
            std::cout << std::setw(15) << name << " ";
            iter_finds(machine, start_tag, end_tag, key, "</td>");
            for (int j = 0; j < 9; j++) {
                string cell = machine.substr(start_tag+key.length(), end_tag-start_tag-key.length());
                if (j == highlighted) {
                    key = "<tdclass=\"highlight\">";
                } else {
                    key = "<td>";
                }
                if (j != 1 && j != 0 && j != 6 && j != 8) {
                    std::cout << std::setw(8) << cell << " ";
                }
                int temp = end_tag;
                iter_finds(machine, start_tag, end_tag, key, "</td>");
            }
            std::cout << "\n";
        } else {
            displacement--;
            quant++;
        }
        iter_finds(data, start, end, "<tr>", "</tr>"); 
    }
    if (!automatic) {
        std::cout << "\nSelect a machine: ";
        int option;
        std::cin >> option;
        if (std::cin.fail() || option - 1 >= quant-1 || option - 1 < 0) {
            std::cout << "Please enter a number between 1 and " << quant-1 << "\n";
            exit(1);
        }
        ssh_command(username, names[option-1], file);
    } 
    return 0;
}

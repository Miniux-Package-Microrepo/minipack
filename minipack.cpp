#include <iostream>
#include <filesystem>
#include <string>

std::string sysrun(std::string command) {
    FILE *output;
    char buffer[100];

    output = popen(command.c_str(), "r");
    if(output==NULL) {
        std::cout << "failed sysrun command: " << command;
    } else {
        while(fgets(buffer,100,output) != NULL) {
            return std::string(buffer);
        }
    }
    pclose(output);
    return "";
}
    

int main(int argc, char *argv[]) {
    if (argc==1) {
        std::cout << "minipack v0.0.2: try 'minipack --help' for more info\n";
    } else if (argc==2) {
        if (std::string(argv[1])=="update") {
            std::cout<<"update system\n";
        } else if (std::string(argv[1])=="install") {
            std::cout<<"missing package name\n";
        } else {
            std::cout<<"unknown option: " << argv[1] << "\n";
        }
    } else if (argc==3) {
        if (std::string(argv[1])=="install"){
            if (sysrun("curl \"https://api.github.com/repos/Miniux-Package-Microrepo/" + std::string(argv[2]) + "/releases/latest\" -s | jq -r .message") == "Not Found\n") {
                std::cout<<"package does not exist: " <<argv[2]<<"\n";
            } else if (sysrun("curl \"https://api.github.com/repos/Miniux-Package-Microrepo/" + std::string(argv[2]) + "/releases/latest\" -s | jq -r .message") == "null\n") {
                std::cout<<"install package: "<<argv[2]<< " " << sysrun("curl \"https://api.github.com/repos/Miniux-Package-Microrepo/" +std::string(argv[2]) + "/releases/latest\" -s | jq -r .tag_name") <<"(y\\n)?>\n";
                std::string usrIn;
                getline(std::cin, usrIn);
                if (usrIn=="y"||usrIn=="Y") {
                    if (std::filesystem::exists("pkgs/" + std::string(argv[2]) + ".min")) {
                        system(("rm pkgs/" + std::string(argv[2]) + ".min").c_str());
                    }
                    system(("cd pkgs && wget \"https://github.com/Miniux-Package-Microrepo/" + std::string(argv[2]) + "/releases/latest/download/" + std::string(argv[2]) + ".min\" && chmod +x " + std::string(argv[2]) + ".min").c_str());
                } else {
                    std::cout << "aborting package download\n";
                    return 0;
                }
            }
        } else {
            std::cout << "unknown option: " << argv[1] << "\n";
        }
    }
    return 0;
}

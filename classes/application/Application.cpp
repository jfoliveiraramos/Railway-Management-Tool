/** @file Application.cpp
 *  @brief Contains the Application Class implementation.
 *  @author -
 *  @bug No known bugs.
 */

#include "Application.h"

void Application::start(){

    state.push(WELCOME_MENU);
    getMenu();
}

void Application::welcomeMenu(){

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << " Welcome to the Analysis Tool for Railway Network Management!" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Click ENTER for the options";
    std::cin.ignore(1000,'\n');

    state.push(READ_MENU);
    getMenu();
}

void Application::readMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "    Analysis Tool for Railway Network Management" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Read files" << std::endl;
        std::cout << "2. Use default files" << std::endl;
        std::cout << "9. Quit" << std::endl << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(!(choice==1 ||choice==2 || choice==9)){
            std::cout << "Invalid option number!";
        }
    }
    while(!(choice==1 ||choice==2 || choice==9));

    switch(choice){
        case 1:{
            std::string file1;
            std::string file2;
            std::cout << "Name of the stations file: ";
            std::cin >> file1;
            std::cin.ignore(1000,'\n');
            std::cout << "Name of the network file: ";
            std::cin >> file2;
            std::cin.ignore(1000,'\n');

            fileReader.setFiles(file1, file2);
            
            if (fileReader.read(stations, railwayNetwork)) {
                fileReader.reset();
                std::cout << std::endl;
                std::cout << "Files specified not found.\n";
                std::cout << "Default files were read instead. (stations.csv, network.csv).";
            }

            state.push(INITIAL_MENU);
            break;
        }
        case 2: {
            fileReader.read(stations, railwayNetwork);
            state.push(INITIAL_MENU);
            break;
        }
    }
    getMenu();
}
void Application::initialMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "    Analysis Tool for Railway Network Management" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Basic Service Metrics" << std::endl;
        std::cout << "2. Operation Cost Optimization" << std::endl;
        std::cout << "3. Reliability and Sensitivity to Line Failures" << std::endl;
        std::cout << "9. Quit" << std::endl << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(!(choice==1 ||choice==2 ||choice==3 ||choice==9)){
            std::cout << "Invalid option number!";
        }
    }
    while(!(choice==1 ||choice==2 ||choice==3 ||choice==9));

    switch(choice){
        case 1:
            state.push(SERVICES_MENU);
            break;
        case 2:
            state.push(COST_MENU);
            break;
        case 3:
            state.push(FAILURE_MENU);
            break;
        case 9:
            exit(0);
    }
    getMenu();
}

void Application::servicesMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "              Basic Services Metrics" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Maximum number of trains between two specific stations" << std::endl;
        std::cout << "2. Pairs of stations that require the most amount of trains" << std::endl;
        std::cout << "3. Top-k municipalities" << std::endl;
        std::cout << "4. Top-k districts" << std::endl;
        std::cout << "5. Maximum number of trains that arrive at a given station" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "9. Quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(choice < 0 || choice == 6 || choice ==7 || choice == 8 || choice >9){
            std::cout << "Invalid option number!";
        }
    }
    while(choice < 0 || choice == 6 || choice ==7 || choice == 8 || choice >9);

    switch(choice){
        case 1: {
            std::string source;
            std::string target;

            std::cout << "Name of the source station: ";
            getline(std::cin, source);
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            auto sourceID = stations[source];
            auto targetID = stations[target];
            if(sourceID== nullptr || targetID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }
            std::cout << basicServices.maxFlow(sourceID->getId(), targetID->getId());
            break;
        }
        case 2: {
            std::cout << "Calculating the pairs..." << std::endl;
            for (auto p: basicServices.optimalPairs()) {
                std::cout << p.first->getStation()->getName() << ", " << p.second->getStation()->getName() << std::endl;
            }
            break;
        }
        case 3: {
            int k_mun;
            std::cout << "Insert the number of municipalities: ";
            std::cin >> k_mun;
            std::cin.ignore();
            for (auto m: basicServices.getMunicipalitiesOrDistricts(true, k_mun)) {
                std::cout << m << std::endl;
            }
            break;
        }
        case 4: {
            int k_dist;
            std::cout << "Insert the number of districts: ";
            std::cin >> k_dist;
            std::cin.ignore();
            for (auto d: basicServices.getMunicipalitiesOrDistricts(false, k_dist)) {
                std::cout << d << std::endl;
            }
            break;
        }
        case 5: {
            std::string target;
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            int targetID=0;
            for (auto s: stations) {
                if (s.first == target) {
                    targetID = s.second->getId();
                }
            }
            if(targetID==0){
                std::cout<< "Invalid station name!";
                break;
            }
            std::cout << basicServices.max_trains_target( targetID);
            break;
        }
        case 0: {
            state.pop();
            break;
        }
        case 9:
            exit(0);
    }
    getMenu();
}

void Application::costMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "             Operation Cost Optimization" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Maximum number of trains between two specific stations" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "9. Quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(!(choice==1 || choice==9 || choice ==0)){
            std::cout << "Invalid option number!";
        }
    }
    while(!(choice==1 || choice==9 || choice ==0));

    switch(choice) {
        case 1: {
            std::string source;
            std::string target;

            std::cout << "Name of the source station: ";
            getline(std::cin, source);
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            auto sourceID = stations[source];
            auto targetID = stations[target];
            if(sourceID== nullptr || targetID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }

            std::pair<int,int> answer;
            answer = costOptimizer.MaxTrainsWithMinCost(sourceID->getId(), targetID->getId());
            std::cout << "The maximum amount of trains: " << answer.first << std::endl;
            std::cout << "Minimum cost: " << answer.second;
            break;
        }
        case 0: {
            state.pop();
            break;
        }
        case 9:
            exit(0);

    }
    getMenu();
}

void Application::failureMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "     Reliability and Sensitivity to Line Failures" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Maximum number of trains between two specific stations" << std::endl;
        std::cout << "2. Top-k most affected stations for each segment" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "9. Quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(!(choice==1 ||choice==2 ||choice==0 ||choice==9)){
            std::cout << "Invalid option number!";
        }
    }
    while(!(choice==1 ||choice==2 ||choice==0 ||choice==9));

    switch(choice){
        case 1:{
            state.push(GRAPHS_MENU);
            break;
        }
        case 2: {
            std::string source;
            std::string target;

            std::cout << "Name of the source station: ";
            getline(std::cin, source);
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            auto sourceID = stations[source];
            auto targetID = stations[target];
            if(sourceID== nullptr || targetID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }

            int k;
            std::cout << "Insert the number of stations: ";
            std::cin >> k;
            std::cin.ignore();
            for (auto m: subgraphService.mostAffectedStations(
                    sourceID->getId(),targetID->getId(), k)) {
                std::cout << m << std::endl;
            }
            break;
        }
        case 0: {
            state.pop();
            break;
        }
        case 9:
            exit(0);
    }
    getMenu();
}

void Application::graphsMenu(){
    do{
        std::cout << std::endl << std::endl << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "     Reliability and Sensitivity to Line Failures" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "1. Delete the station" << std::endl;
        std::cout << "2. Delete the edge" << std::endl;
        std::cout << "3. Delete the service" << std::endl;
        std::cout << "4. Process" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "9. Quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(1000,'\n');
        if(!(choice==1 ||choice==2 ||choice==0 ||choice==9 || choice ==3 || choice == 4)){
            std::cout << "Invalid option number!";
        }
    }
    while(!(choice==1 ||choice==2 ||choice==0 ||choice==9 || choice ==3 || choice == 4));

    switch(choice){
        case 1: {
            std::string station;
            std::cout << "Name of the station: ";
            getline(std::cin, station);

            auto stationID = stations[station];
            if(stationID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }

            subgraphService.deleteStation(stationID->getId());
            break;
        }
        case 2:{
            std::string source;
            std::string target;

            std::cout << "Name of the source station: ";
            getline(std::cin, source);
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            auto sourceID = stations[source];
            auto targetID = stations[target];
            if(sourceID== nullptr || targetID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }

            subgraphService.deleteEdge(sourceID->getId(), targetID->getId());
            break;
        }
        case 3:{
            std::string service;
            std::cout << "Service type: ";
            getline(std::cin, service);
            std::cout << service;

            if(service != "STANDARD" && service != "ALFA PENDULAR"){
                std::cout<< "Invalid service type!";
                break;
            }
            if(service == "STANDARD"){
                ServiceType standard = STANDARD;
                subgraphService.deleteService(standard);
            }
            else{
                ServiceType alfa = ALFA_PENDULAR;
                subgraphService.deleteService(alfa);
            }
            break;
        }
        case 4: {
            std::string source;
            std::string target;

            std::cout << "Name of the source station: ";
            getline(std::cin, source);
            std::cout << "Name of the target station: ";
            getline(std::cin, target);

            auto sourceID = stations[source];
            auto targetID = stations[target];
            if(sourceID== nullptr || targetID== nullptr){
                std::cout<< "Invalid stations names!";
                break;
            }

            std::cout << subgraphService.maxFlow(sourceID->getId(), targetID->getId());
            break;
        }
        case 0: {
            state.pop();
            break;
        }
        case 9:
            exit(0);
    }
    getMenu();
}

void Application::getMenu(){
    if(!state.empty()){
        States states = state.top();
        switch(states){
            case WELCOME_MENU:
                welcomeMenu();
                break;
            case READ_MENU:
                readMenu();
                break;
            case INITIAL_MENU:
                initialMenu();
                break;
            case SERVICES_MENU:
                servicesMenu();
                break;
            case COST_MENU:
                costMenu();
                break;
            case FAILURE_MENU:
                failureMenu();
                break;
            case GRAPHS_MENU:
                graphsMenu();
                break;
        }
    }
}

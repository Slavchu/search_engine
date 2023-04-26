#include <json_converter.hpp>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
using namespace JSON;

std::string config::version = "";
std::string config::name = "noname";
int config::maxResponses = 0;

ConverterJSON::ConverterJSON(){
    std::ifstream cfg(std::filesystem::current_path().string() + "/config.json"),
    req(std::filesystem::current_path().string() + "/requests.json");
    
    if(!cfg.is_open() || !req.is_open()) {isOpen = false ; return;}
    isOpen = true;
    cfg >> fileConfig;
    req >> fileRequests;
    
    
    
    config::maxResponses = fileConfig["config"]["max_responses"];
    config::name = fileConfig["config"]["name"];
    config::version = fileConfig["config"]["version"];
}
std::vector <std::string > ConverterJSON::getTextDocument() const{
    if(fileConfig.find("files") == fileConfig.end() || !fileConfig.find("files").value().size()) 
    int file_count = fileConfig.find("files").value().size();
    std::vector<std::string> filenames =  fileConfig.find("files").value();
    std::vector<std::string> result;

    for(auto &it: filenames){
        if(it.find("*/") != std::string::npos){
            it.replace(it.find("*/"), 2, std::filesystem::current_path().string() + "/");
        }
        std::ifstream file(it);
        std::stringstream ss;
        if(!file.is_open()) {std::cout << it << std::endl << "Not oppened" << std::endl;}

        ss << file.rdbuf();
        result.push_back(ss.str());
    }
    
    return result;
    


}

std::vector <std::string> ConverterJSON::getRequests() const{
    
    std::vector<std::string> res =fileRequests.find("requests").value();
    
    if(res.size() > config::maxResponses)
        res.erase(res.begin()+config::getMaxResponses(), res.end());
    return res;
}

void ConverterJSON::putAnswers(const std::vector<std::vector <answer>>& answers) const{
    std::ofstream ans("answers.json");
    ans << "{\n"<<
    "\t\"answers:\":{\n";
    for(int i = 0; i < answers.size(); i++){
        std::string request = "request" + std::to_string(i/100) + std::to_string((i%100)/10) + std::to_string((i%100 % 10));
        ans << "\t\t" << "\"" << request << "\":{\n";
        
        ans << "\t\t\t" << "\"result\": " << (answers[i].empty()? "\"false\"" : "\"true\",") <<  std::endl;
        
        for(int j = 0; j < answers[i].size(); j++){
            ans << "\t\t\t\t"<<"\"docid\": " << answers[i][j].first << ",\t\"rank\" : " << answers[i][j].second;
            if(j != answers[i].size()-1) ans <<",";
            ans << "\n";
        }

        ans << "\t\t}";
        if(i != answers.size()-1) ans << ",";
        ans << "\n";
    }
    ans << "\t}\n}";
    
    
}


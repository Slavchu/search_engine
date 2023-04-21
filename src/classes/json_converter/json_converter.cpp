#include <json_converter.hpp>
#include <sstream>
#include <fstream>
#include <filesystem>
using namespace JSON;

std::string config::version = "";
std::string config::name = "noname";
int config::maxResponses = 0;

ConverterJSON::ConverterJSON(){
    std::ifstream cfg(std::filesystem::current_path().string() + "/config.json"),
    req(std::filesystem::current_path().string() + "/requests.json");
    
    if(!cfg.is_open()) throw json_exception("config file not found");
    else if(!req.is_open()) throw json_exception("request file not found");
    cfg >> fileConfig;
    req >> fileRequests;

    
    if(fileConfig.find("config") == fileConfig.end()){
        throw json_exception("config error");
        
    }
    if(fileConfig["config"]["name"].is_null() || fileConfig["config"]["version"].is_null() || fileConfig["config"]["max_responses"].is_null()){
        throw json_exception("not enough config data");
    }
    config::maxResponses = fileConfig["config"]["max_responses"];
    config::name = fileConfig["config"]["name"];
    config::version = fileConfig["config"]["version"];
}
std::vector <std::string > ConverterJSON::getTextDocument(){
    if(fileConfig.find("files") == fileConfig.end() || !fileConfig.find("files").value().size()) 
        throw json_exception("no files set");
    file_count = fileConfig.find("files").value().size();
    std::vector<std::string> filenames =  fileConfig.find("files").value();
    std::vector<std::string> result;
    for(int i = 0; i < filenames.size(); i++){
        std::ifstream file(filenames[i]);
        std::stringstream ss;
        if(!file.is_open()) throw json_exception(filenames[i] + "\n File not found");

        ss << file.rdbuf();
        result.push_back(ss.str());
    }
    
    return result;
    


}

std::vector <std::string> ConverterJSON::getRequests(){
    if(fileRequests.find("requests") == fileRequests.end() || !fileRequests.find("requests").value().size())
        throw json_exception("no requests found");
    std::vector<std::string> res =fileRequests.find("requests").value();
    
    if(res.size() > config::maxResponses)
        res.erase(res.begin()+config::getMaxResponses(), res.end());
    return res;
}

void ConverterJSON::putAnswers(std::vector<std::vector <answer>> answers){
    std::ofstream ans("answers.json");
    ans << "{\n"<<
    "\t\"answers:\":{\n";
    for(int i = 0; i < answers.size(); i++){
        std::string request = "file" + std::to_string(i/100) + std::to_string((i%100)/10) + std::to_string((i%100 % 10));
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


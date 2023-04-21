#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <exception>
namespace JSON{
    typedef std::pair<size_t, float> answer;
    
    class ConverterJSON;
    
    class config{
        friend ConverterJSON;
        static std::string version;
        static std::string name;
        static int maxResponses;
        public:
            static std::string getVersion(){return version;}
            static std::string getName(){return name;}
            static int getMaxResponses(){
                return maxResponses;
            }

    };
    class ConverterJSON{
        
        private:
            nlohmann::json fileConfig;
            nlohmann::json fileRequests;
            size_t file_count, request_count;
        public:
            ConverterJSON();
            std::vector <std::string> getTextDocument();
            std::vector <std::string> getRequests();
            void putAnswers(std::vector<std::vector<answer>> answers);
    };
    class json_exception: public std::exception{
        std::string error;
        public:
        json_exception(std::string err) :error(err){};
        const char * what() const noexcept override{
            return error.c_str();
        }
    };

}
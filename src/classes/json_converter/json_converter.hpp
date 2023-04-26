#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
namespace JSON{
    typedef std::pair<size_t, float> answer;
    
    class ConverterJSON;
    
    class config{
        friend ConverterJSON;
        static std::string version;
        static std::string name;
        static int maxResponses;
        public:
            static std::string getVersion() {return version;}
            static std::string getName()  {return name;}
            static int getMaxResponses() {return maxResponses;}

    };
    class ConverterJSON{
        
        private:
            nlohmann::json fileConfig;
            nlohmann::json fileRequests;
          
            bool isOpen;

        public:
            ConverterJSON();
            std::vector <std::string> getTextDocument() const;
            std::vector <std::string> getRequests() const;
            bool isOpened() const{return isOpen;}

            void putAnswers(const std::vector<std::vector<answer>>& answers) const;
    };
    

}
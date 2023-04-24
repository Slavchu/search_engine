#include <iostream>
#include <json_converter.hpp>
#include <inverted_index.hpp>
#include <search_server.hpp>
#include <map>

int main(int argc, char **argv){
    
    JSON::ConverterJSON converter;
    if(!converter.isOpened()){
        std::cerr << "Can't open config or request file\n";
        return 0;
    }
    std::cout << JSON::config::getName() << std::endl << JSON::config::getVersion() << std::endl;
    inverted_index::InvertedIndex invIndex;

    invIndex.updateDocumentBase(converter.getTextDocument());
    auto requests = converter.getRequests();
    size_t req_size = (requests.size() < JSON::config::getMaxResponses()) ? requests.size() : JSON::config::getMaxResponses();
    search_server::SearchServer searchServ(invIndex);
    std::vector<std::vector<std::pair<size_t, float>>> answers;
    for (auto it : requests)
    {
        std::vector<std::string> request;
        std::stringstream ss;
        ss << it;
        std::string word;
        while (!ss.eof())
        {
            ss >> word;
            request.push_back(inverted_index::to_lover_erase(word));
        }
        auto answer = searchServ.search(request);
        std::vector<std::pair<size_t, float>> vecAnswer;
        for (auto it : answer)
        {
            vecAnswer.push_back(std::make_pair(it.doc_id, it.rank));
        }
        answers.push_back(vecAnswer);
    }
    converter.putAnswers(answers);
}
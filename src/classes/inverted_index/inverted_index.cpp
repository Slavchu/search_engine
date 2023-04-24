#include <inverted_index.hpp>
#include <algorithm>
#include <thread>
#include <iostream>
#include <string>
using namespace inverted_index;
void InvertedIndex::indexer(std::string document, int doc_ind)
{
    using namespace std;

    string word;
    std::stringstream doc;
    doc << document;
    while (!doc.eof()){
        doc >> word;
        word = to_lover_erase(word);

        if (freqDictionary.find(word) == freqDictionary.end()){
            vector<Entry> vec;
            vec.push_back(Entry(doc_ind, 1));
            freqDictionary[word] = vec;
        }
        else{
            vector<Entry> &vec = freqDictionary[word];
            auto it = find(vec.begin(), vec.end(), doc_ind);
            if (it != vec.end())
            {
                it->count++;
            }
            else
            {
                vec.push_back(Entry(doc_ind, 1));
            }
        }
    }
}
void InvertedIndex::updateDocumentBase(std::vector<std::string> documents){
    std::vector <std::thread> th;
    for(int i = 0; i < documents.size(); i++){
        th.push_back(std::thread(&InvertedIndex::indexer, this, documents[i], i));
    }
    for(int i = 0; i < documents.size(); i++){
        th[i].join();
    }
}
std::vector <Entry>InvertedIndex::getWordCount(std::string word) const{
    if(freqDictionary.find(word) == freqDictionary.end())
        return std::vector<Entry>();
    return freqDictionary.at(word);
}
std::string inverted_index::to_lover_erase(const std::string str){
        std::string result = str;
        int pos = 0;
        for(auto &it : result){
            if(!isalpha(it)){
                result.erase(pos, 1);
            }
            it = tolower(it);
            pos++;
        }
        return result;
}
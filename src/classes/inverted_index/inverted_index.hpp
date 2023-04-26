#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>


namespace inverted_index{
    std::string to_lover_erase(const std::string str);
    struct Entry{
        size_t document_id, count;
        Entry(size_t document_id, size_t count){
            this->count = count;
            this->document_id = document_id;
        }
        
        bool operator ==(const Entry& other) const {
            return (document_id == other.document_id &&
             count == other.count);
        }
        bool operator==(int document_id) const{
            return this->document_id == document_id;
        }
        bool operator<(const Entry& other) const{
            return document_id < other.document_id;
        }
    };
    class InvertedIndex{
            std::map<std::string, std::vector <Entry>> freqDictionary;
            void indexer(const std::string& document, int doc_ind);
        public:
            void updateDocumentBase(const std::vector <std::string>& documents);
            std::vector<Entry> getWordCount(const std::string& word) const;

    };
    
}

#pragma once
#include <inverted_index.hpp>
#include <gtest/gtest.h>
namespace search_server{
    struct RelativeIndex{
        size_t doc_id;
        float rank;
        RelativeIndex(size_t doc_id, float rank){
            this->doc_id = doc_id;
            this->rank = rank;
        }
        bool operator>(const RelativeIndex& oth) const{
            if(rank == oth.rank) return doc_id < oth.doc_id;
            return rank > oth.rank;
        }
         bool operator==(const RelativeIndex& oth) const{
            return rank == oth.rank && doc_id == oth.doc_id;
        }
    };
    class SearchServer{
        private:
            inverted_index::InvertedIndex mInd;
        public:
            SearchServer(inverted_index::InvertedIndex &ind);
            std::vector <RelativeIndex> search(const std::vector<std::string>& request) const;
    };

}
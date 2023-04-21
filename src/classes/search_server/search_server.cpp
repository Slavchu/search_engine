#include <search_server.hpp>
#include <map>
#include <algorithm>
using namespace search_server;

SearchServer::SearchServer(inverted_index::InvertedIndex &ind){
    mInd = ind;
}

std::vector <RelativeIndex> SearchServer::search(std::vector<std::string> request){  
       std::vector <RelativeIndex> res;
       std::map <size_t, size_t> absolute;
       
       for (auto it : request){

            auto wordCount = mInd.getWordCount(it);
            if (wordCount.empty())
               continue;
            for (int i = 0; i < wordCount.size(); i++){
                 if (absolute.find(wordCount[i].document_id) != absolute.end())
                      absolute[wordCount[i].document_id] += wordCount[i].count;
                 else
                      absolute[wordCount[i].document_id] = wordCount[i].count;
            }
       }

       if(absolute.empty()) return res;

       size_t max = 0;

       for(auto it : absolute){
            max = (max < it.second)? it.second: max;
       }

       for(auto it : absolute){
            res.push_back(RelativeIndex(it.first, (float)it.second/(float)max));
       }


       std::sort(res.begin(), res.end(), std::greater());
       return res;
}
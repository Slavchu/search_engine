#include <iostream>
#include <json_converter.hpp>
#include <inverted_index.hpp>
#include <search_server.hpp>
#include <sstream>
#include <map>
#include <gtest/gtest.h>

void TestInvertedIndexFunctionality(
    const std::vector<std::string> &docs,
    const std::vector<std::string> &requests,
    const std::vector<std::vector<inverted_index::Entry>> &expected)
{
    
    std::vector<std::vector<inverted_index::Entry>> result;
    inverted_index::InvertedIndex idx;
    idx.updateDocumentBase(docs);
    for (auto &request : requests)
    {
        std::vector<inverted_index::Entry> word_count = idx.getWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);

}
TEST(TestCaseInvertedIndex, TestBasic)
{
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"};
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<inverted_index::Entry>> expected = {
        {{0, 1}}, {{0, 1}, {1, 3}}};
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2)
{
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"};
    const std::vector<std::string> requests = {"milk", "water", "cappuchino"};
    const std::vector<std::vector<inverted_index::Entry>> expected = {
        {{0, 4}, {1, 1}, {2, 5}}, {{0, 3}, {1, 2}, {2, 5}}, {{3, 1}}};
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord)
{
    using namespace std;
    const std::vector<string> docs = {
        "a b c d e f g h i j k l",
        "statement"};
    const vector<string> requests = {"m", "statement"};
    const vector<vector<inverted_index::Entry>> expected = {
        {}, {{1, 1}}};
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseSearchServer, TestSimple)
{
    using namespace std;
    const std::vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"};
    const std::vector<string> request = {"milk", "water", "sugar"};
    const std::vector<vector<search_server::RelativeIndex>> expected = {
        {{2, 1},
         {0, 0.7},
         {1, 0.3}},
        };
    inverted_index::InvertedIndex idx;
    idx.updateDocumentBase(docs);
    search_server::SearchServer srv(idx);
    std::vector<vector<search_server::RelativeIndex>> result; 
    result.push_back(srv.search(request));
    ASSERT_EQ(result, expected); 
}

TEST(TestCaseSearchServer, TestTop5)
{
    using namespace std;
    const vector<string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland"
    };
 
    const vector<string> request = {"moscow", "is", "the", "capital", "of", "russia"};
    const std::vector<vector<search_server::RelativeIndex>> expected = {
        {{7, 1},
         {14, 1},
         {0, 0.666666687},
         {1, 0.666666687},
         {2, 0.666666687}}};
    inverted_index::InvertedIndex idx;
    idx.updateDocumentBase(docs);
    search_server::SearchServer srv(idx);
    std::vector<vector<search_server::RelativeIndex>> result; result.push_back(srv.search(request));
    result[0].erase(result[0].begin()+5, result[0].end());
    ASSERT_EQ(result, expected);
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    try{
        JSON::ConverterJSON converter;
        inverted_index::InvertedIndex invIndex;
        
        invIndex.updateDocumentBase(converter.getTextDocument());
        auto requests = converter.getRequests();
        size_t req_size = (requests.size() < JSON::config::getMaxResponses() ) ? requests.size() : JSON::config::getMaxResponses();
        search_server::SearchServer searchServ(invIndex);
        std::vector<std::vector<std::pair<size_t, float>>> answers;
        for(auto it: requests){
            std::vector <std::string> request;
            std::stringstream ss;
            ss << it;
            std::string word;
            while(!ss.eof()){
                ss >> word;
                request.push_back(inverted_index::to_lower_erase(word));
            }
            auto answer = searchServ.search(request);
            std::vector <std::pair <size_t,float>> vecAnswer;
            for(auto it: answer){
                vecAnswer.push_back(std::make_pair(it.doc_id, it.rank));
            }
            answers.push_back(vecAnswer);
        }
        converter.putAnswers(answers);
        
        

    } catch(JSON::json_exception &e){
        std::cerr <<"JsonException:" << e.what() << std::endl;
        return -1;
    }
    return RUN_ALL_TESTS();
}
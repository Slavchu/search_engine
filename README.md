# SEARCH ENGINE BY SLAVCHOO

___This is file search engine,that can help you quickly find files, based on your requests___

## Building 
For build use commands:
```
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release  -S{PROJECT_DIRECTORY} -B{BUILD_DIRECTORY} -G Ninja
cmake --build {BUILD_DIRECTORY} --config Release --target all --
```
Location of output executable is {BUILD_DIRECTORY}/output_build/

> NOTE: Program won't start without configuration files in executable directory: config.json and requests.json

## Configuration
For configuration search_engine you need to create json files:</br>
>config.json</br>
>requests.json </br>

## Config JSON
Config file contains two main fields: </br>
- "config" with parameters "name"(string), "version"(string) and "max_responses"(integer)</br>
- array "files" with path to files, where search engine will search requests. 

***Example of config.json:***
```json
{
    "config":{
        "name" : "Search_Engine",
        "version" : "1.0",
        "max_responses" : 5


    },
    "files":[
      "/home/examplefile.txt",
      "/home/examplefile2.txt"
    ]
}
```
> ***Note: you can type "*/filepath" to use relative path to file** 

## Request JSON
Request file contains all your request and will give result to file ___answers.json___. "request.json" is consisted of the array "requests" with google like requests. </br>

***Example of requests.json***
```json
{
    "requests": [
        "cow gave milk to the chicken, article ",
        "request.json example",
        "where to hide a body"
    ]
}
```

## Answers JSON
This file generates by program and contains results on requests.json file. Has field ___"answers"___, where are writen all the responses. Every response field has ___"result"___ variable. In case program haven't found result, value of variable will be false, and there will not be aby other field. In case result was found, there will be field ___"relevance"___, that contains fields ___"docid", that are just number of document, you wrote in config.json and their ranks. Max value of rank is 1. Result are sorted from higher to lower value by documents relevance.</br>
***Example of answers.json:***
```json
{
    "answers": {
        "request001": {
            "result":"true",
            "relevance": {
                "docid": 0, “rank” : 1,
                "docid": 1, “rank” : 0.897,
                "docid": 2, “rank” : 0.750,
                "docid": 3, “rank” : 0.670,
                "docid": 4, “rank” : 0.561
            }
        },
        "request002": {
            "result": "true",
            "relevance":
                "docid": 0, “rank” : 1
        },
        "request003": {
            "result": "false"
        }
    }
}
```

> Note: careful, in answers.json file can be fields with same name!

</br></br></br></br>
# Code documentation
## Namespaces:</br></br>



### ***JSON*** </br>
> Classes & structures: 
> - ConverterJSON - can read information from _config_ and _requests_ json files, writes result to _answers.json_
> - config - static class that contains infromation about _name_, _version_ and _max_responses_ parameters. Only ConverterJSON class can change them, from other places you can just get their values;

> Types:
> - answer - typedef std::pair<size_t, float>  

### ***inverted_index*** </br>
> Classes & structures: 
> - InvertedIndex - gets a count of each word from files. 
> - Entry - structure, that contains document id and count of word.

>Functions: 
> - std::string to_lover_erase(const std::string str) - returns string without small characters and signs (",.!(); etc...)

### ***search_server***
> Classes & structures: 
> - SearchServer - searches requests and gives sorted result
> - RelativeIndex - struct, that contains document and its document id for request </br>

# Classes & structures
## ConverterJSON
- ***ConverterJSON();*** </br>
    default constructor, reads config and cheks request existance
- ***std::vector <std::string> getTextDocument() const***</br>
returns text of documents, writen in config. 
- ***std::vector <std::string> getRequests() const*** </br>
returns requests from _requests.json_
- ***bool isOpened() const*** </br>
cheks if all files have been opened
- ***void putAnswers(std::vector<std::vector<answer>> answers) const***</br>
puts all answers in _answers.json_ file
## config
- ***static std::string getVersion()*** </br> returns version, read from _config.json_
- ***static std::string getName()*** </br> returns name, read from _config.json_
- ***static int getMaxResponses()*** </br> returns num of max responses, read from _config.json_

> **Note:** config json gets data after initialization **ConvertJSON** class. Only **ConvertJSON** has access to **config** data

## InvertedIndex

```c++ 
void updateDocumentBase(std::vector <std::string> documents) 
```
</br>
updates database of each word count in document. Input is std::vector of std::string, that contains texts of each document. Index of text is document id. Function is multithreaded.</br>

```c++
std::vector<Entry> getWordCount(std::string word) 
``` 
</br>
returns array of class inverted_index::Entry for word. Contains data of document id of document, where word was found and num of it in the document. vector is sorted by count from lowest to greatest

## Entry
``` c++
size_t document_id
```
document id
</br></br></br>

```c++
size_t count
``` 
count words appearings in the document
</br></br></br>

```c++
Entry (size_t document_id, size_t count)
```
class constructor
</br></br></br>

```c++
bool operator ==(const Entry& other) const
```
checks equality of structures
</br></br></br>

```c++
bool operator==(int) const
```
checks equality of documents id
</br></br></br>

```c++
bool operator<(const Entry& other) const
```
checks if the rank of current structure is lower then other one
</br></br></br>

## SearchServer
```c++
SearchServer(inverted_index::InvertedIndex &ind);
```
Constructor. Takes reference to InvertedIndex for documents you want to search
</br></br></br>

```c++
std::vector <RelativeIndex> search(std::vector<std::string> request) const
```
Returns response on request. Each word of request is separate element of vector of strings. Response is vector of **RelativeIndex**, sorted by rank from greatest to lowest, and by document id from lowest to greatest in case of rank equality.

## RelativeIndex
```c++
size_t doc_id;
float rank;
```
document id and rank of document for request.
</br></br></br>
```c++
size_t doc_id;
float rank;
```
document id and rank of document for request.
</br></br></br>
```c++
RelativeIndex(size_t doc_id, float rank)
```
Constructor. Takes document_id and rank for input
</br></br></br>
```c++
bool operator>(const RelativeIndex& oth) const
```
Retruns true if current structs rank is greater than other. In case of ranks equality returns true if document_id is lower than other. 
</br></br></br>

```c++
bool operator==(const RelativeIndex& oth) const
```
Returns true if structs variables values are equal
</br></br></br>

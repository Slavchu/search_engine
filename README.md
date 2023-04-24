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
> - RelativeIndex - struct, that contains document and its document id for request

 
 


# SEARCH ENGINE BY SLAVCHOO

for build use commands:
```
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release  -S{PROJECT_DIRECTORY} -B{BUILD_DIRECTORY} -G Ninja
cmake --build {BUILD_DIRECTORY} --config Release --target all --
```
Location of output executable is {BUILD_DIRECTORY}/output_build/

NOTE: Program won't start without configuration files in executable directory: config.json and requests.json


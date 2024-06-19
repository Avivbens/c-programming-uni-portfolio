#!/bin/bash
project_name=$1

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    ./dist/$project_name/$project_name.exe
else
    ./dist/$project_name/$project_name
fi

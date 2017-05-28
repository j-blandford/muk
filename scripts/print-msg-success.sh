#!/bin/bash

# thanks to Kevin Lange for the following helper class!
function cpadd {
    word="$1"
    while [ ${#word} -lt $2 ]; do
        word="$word$3";
        if [ ${#word} -lt $2 ]; then
            word="$3$word"
        fi;
    done;
    echo "$word";
}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CMD=`cpadd "$1" 8 " "`
echo -e -n "\r\033[5;94m${CMD}${2}\033[0m\n"

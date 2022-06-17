#!/bin/sh

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "SCript Path: ${SCRIPT_PATH}"
MISC_PATH=${SCRIPT_PATH}/../misc

curl http://thyme.apnic.net/current/data-raw-table -o ${MISC_PATH}/data-raw-table.txt
curl http://thyme.apnic.net/current/data-used-autnums -o ${MISC_PATH}/data-used-autnums.txt

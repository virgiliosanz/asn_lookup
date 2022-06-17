#!/bin/sh

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PRJ_PATH=${SCRIPT_PATH}/..
#LOG_FILES=misc/logs/cocinario/*.gz
LOG_FILES=${PRJ_PATH}/misc/logs/sample_logs/access.log.19.gz

gzcat ${LOG_FILES} | ${PRJ_PATH}/bin/add_asn_to_log 2> /tmp/add.log | ${PRJ_PATH}/bin/sum_asn_traffic 2> /tmp/sum.log | ${PRJ_PATH}/bin/fill_asn_names 2> /tmp/names.log > asn_traffic_${MONTH}.csv

echo "FINISHED --- Logs at /tmp"


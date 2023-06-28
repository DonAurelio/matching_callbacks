#!/bin/bash -x

set -e

# metababel -d interval_model.yaml -t SOURCE -o ./SOURCE.interval

gcc -o ./SOURCE.interval/btx_source.so \
    ./SOURCE.interval/*.c \
    ./SOURCE.interval/metababel/*.c \
    -I ../include -I ./SOURCE.interval/ \
    $(pkg-config --cflags babeltrace2) $(pkg-config --libs babeltrace2) -Wall -Werror -fpic --shared

babeltrace2 --plugin-path=SOURCE.interval \
            --component=source.metababel_source.btx \
            --component=sink.text.details 

# metababel -u interval_model.yaml -d interval_model.yaml -t FILTER -o ./FILTER.interval

gcc -o ./FILTER.interval/btx_filter.so \
    ./FILTER.interval/*.c \
    ./FILTER.interval/metababel/*.c \
    -I ../include -I ./FILTER.interval/ \
    $(pkg-config --cflags babeltrace2) $(pkg-config --libs babeltrace2) -Wall -Werror -fpic --shared

babeltrace2 --plugin-path=SOURCE.interval:FILTER.interval  \
            --component=source.metababel_source.btx \
            --component=filter.metababel_filter.btx

# echo "Clean"
# rm ./SOURCE.interval/btx_* ./SOURCE.interval/metababel/btx_* ./SOURCE.interval/metababel/metababel.h
# rmdir ./SOURCE.interval/metababel

# rm ./FILTER.interval/btx_* ./FILTER.interval/metababel/btx_* ./FILTER.interval/metababel/metababel.h
# rmdir ./FILTER.interval/metababel

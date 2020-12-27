#!/bin/bash

SRC=$1
DST="${1%.*}".mp3

echo "Converting $SRC to $DST"
timidity $SRC -Ow -o - | ffmpeg -i - -acodec libmp3lame -ab 128k $DST

#!/bin/sh
for i in `ls *.[c,h]`; do indent -bl -nfc1 -ncdb $i && rm -f $i.BAK; done
rm -f *~


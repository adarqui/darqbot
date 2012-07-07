#!/bin/sh

#for i in `ls *.[c,h]`; do indent -bl -nfc1 -ncdb $i && rm -f $i.BAK; done
for i in `ls *.[c,h]`; do indent -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1 $i && rm -f $i.BAK; done
rm -f *~


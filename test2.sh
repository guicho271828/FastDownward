#!/bin/bash

./test.sh | awk '
/Task/{p=$2}
/Nick/{n=$2}
/Evaluated/{e=$2}
/Actual search time/{t=$4}
/Plan valid/{print p" "n" "e" "t" "e/t; fflush()} ' | tee log

sort -k1,1 -k3h,3  log | column -t 

for i in `seq 1 1 12`; do ./test > output.txt & done ; watch -gn 10 grep "tries" output.txt; killall ./calc;

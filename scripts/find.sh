for i in `seq 1 1 12`; do ./calc > output.txt & done ; watch -gn 10 grep "tries" output.txt; killall ./calc;
#                 ^^ numer of cpu cores you want to use
printSymbolFromInt() {
    # get corresponding symbol from int
    if [ "$1" = "0" ]; then
        SYMBOL=""
    else
        if [ "$1" = "1" ]; then
        SYMBOL="+"
        else
            SYMBOL="-"
        fi
    fi
    printf "$SYMBOL"
}

printNumberAndOperator() {
    # output number
    printf "$1"
    # get first symbol in int
    FIRST_SYMBOL_INT=`expr $2 % 3`
    # print the corresponding symbol
    printSymbolFromInt $FIRST_SYMBOL_INT
}


for number in `seq 0 242`
# iterate among required numbers
do
    for digit in `seq 1 5`
    # print 1X2X3X4X5X, where X is a operator without \n
    do
        printNumberAndOperator $digit $number
        number=`expr $number / 3`
    done
    # print the last 6 with a \n
    echo 6
done

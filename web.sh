stty -icanon min 1
char=$(head -c 1 | tr -d '\n')
stty icanon

echo "You pressed: $char"

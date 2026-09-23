#!/bin/bash
count=$#

if [ $count -eq 0 ]; then
    echo "кол-во 0"
    echo "ср арифм 0"
    exit 0
fi

sum=0
for num in "$@"; do
    sum=$((sum + num))
done

avg=$((sum / count))

echo "кол-во $count"
echo "ср арифм $avg"

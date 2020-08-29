#!/bin/bash

funkcja()
{

if [ -d ~/$1 ]; then
for j in {1..5}
do
touch ~/$1/$j.txt
ps $j > ~/$1/$j.txt
done
echo "Stworzono 5 roznych plikow"
else
echo "Brak katalogu tworzę nowy"
mkdir $1
echo "Uworzono katalog o nazwie $1"
echo "Uruchom program ponownie"
fi
}

funkcja $1
#!/bin/bash

echo Please enter filename to push onto pintos
read filename
pintos -q rm $filename
pintos -p ../examples/$filename -a $filename -- -q 

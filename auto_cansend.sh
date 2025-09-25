#!/bin/bash
while true; do
    cansend vcan0 123#1122334455667788
    sleep 1
    cansend vcan0 456#AABBCCDDEEFF0011
    sleep 1
done
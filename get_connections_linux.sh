#!/bin/bash

# List TCP connections
ss -tupn > ./Data/connections.txt

# List UDP connections
ss -uapn >> ./Data/connections.txt

# List Processes
ps -eo pid,comm > ./Data/processes.txt

@echo off

:: List TCP connections
netstat -ano | findstr "TCP" > .\Data\connections.txt

:: List UDP connections
netstat -ano | findstr "UDP" >> .\Data\connections.txt

:: List Processes
tasklist /fo csv /nh > .\Data\processes.txt

echo Script executed successfully.

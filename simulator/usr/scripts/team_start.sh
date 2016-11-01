#!/bin/bash


source setenv.sh
echo RTDB-Client starting...
./$SOCCERROOT/usr/bin/kogmo_rtdb_man 
mkdir -p ~/logs
#tcptool $1 129.187.151.69 > ~/logs/tcpclient.log &
echo "Wait 30 seconds before starting your application!!"
echo Press \<Return\> to Stop Client.
#read STOPPING
../bin/RoSoSimulator | ../bin/referee_gui

echo Client stopping...
#killall tcptool
$SOCCERROOT/usr/bin/kogmo_rtdb_man -k >/dev/null  2>/dev/null
$SOCCERROOT/usr/bin/kogmo_rtdb_kill -a >/dev/null 2>/dev/null
echo Client stopped!!

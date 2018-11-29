#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
case "$1" in
  recoveryPLC)
 	echo "recovery plc data ..."
	if [ -d "/rbctrl/data/user" ]; then
	    echo "the dir have been created!";
	else
	    mkdir "/rbctrl/data/user" -p
	fi
	cp ${plcPath}/plc.dat /rbctrl/data/user/
	;;
  recoveryJBI)
	echo "recovery jbi files ..."
	if [ -d "/rbctrl/program" ]; then
	    echo "the dir have been created!";
	else
	    mkdir "/rbctrl/program" -p
	fi
	cp ${jbiPath}/*.jbi /rbctrl/program/
	;;
  recoveryDatabase)
	echo "backupDatabase..."
	;;
  *)
	echo "Usage: $0 {recoveryPLC|recoveryJBI|recoveryDatabase}"
	;;
esac

#!/bin/sh
plcUDiskPath="/mnt/udisk/rbctrl/data/sys"
plcSysPath="/rbctrl/data/sys"
jbiPath="/mnt/udisk/rbctrl/program/"
upgradePath="/update"
case "$1" in
  recoveryPLC)
 	echo "recovery plc data ..."
	if [ -d "/rbctrl/data/sys" ]; then
	    echo "the dir have been created!";
	else
	    mkdir "/rbctrl/data/sys" -p
	fi
	cp ${plcUDiskPath}/plc.dat $plcSysPath
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
	echo "recovery Database ..."
	cd $upgradePath
	/etc/init.d/rbctrl.sh stop
	cp /mnt/udisk/db/parameters.bak /update
	./upgrader recoveryDatabase -plugin rbteach -plugin tslib &
	;;
  recoveryFactoryApplication)
	echo "recovery factory application ..."
	cd $upgradePath
	/etc/init.d/rbctrl.sh stop
	./upgrader recoveryFactoryApplication
	;;
  *)
	echo "Usage: $0 {recoveryPLC|recoveryJBI|recoveryDatabase|recoveryFactoryApplication}"
	;;
esac

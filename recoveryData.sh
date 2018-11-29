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
	echo "recovery Database ..."
	cd /rbctrl
	/etc/init.d/rbctrl.sh stop
	cp /mnt/udisk/db/parameters.bak /update
	sleep 1
	export OPERATE_TYPE=recoveryDatabase
	./upgrader -plugin rbteach -plugin tslib &
	;;
  recoveryFactoryApplication)
	echo "recovery factory application ..."
	cd /update
	/etc/init.d/rbctrl.sh stop
	sleep 1
	export OPERATE_TYPE=recoveryFactoryApplication
	./upgrader -plugin rbteach -plugin tslib &
	;;
  *)
	echo "Usage: $0 {recoveryPLC|recoveryJBI|recoveryDatabase|recoveryFactoryApplication}"
	;;
esac

#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
case "$1" in
  backupPLC)
 	echo "backup plc data ..."
	if [ -d $plcPath ]; then
    echo "the dir have been created!";
	else
    mkdir $plcPath -p
	fi
	cp /rbctrl/plc.dat $plcPath
	;;
  backupJBI)
	echo "backup jbi files ..."
	if [ -d $jbiPath ]; then
    echo "the dir have been created!";
	else
    mkdir $jbiPath -p
	fi
	cp /rbctrl/*.jbi $jbiPath
	;;
  backupDatabase)
	echo "backupDatabase..."
	cd /rbctrl
	killall -9 cobot
	sleep 1 
	export OPERATE_TYPE=backupDatabase
	./upgrader -plugin rbteach -plugin tslib &
#	./cobot -plugin rbteach -plugin tslib &
	cd ..
	;;
  backupFactoryApplication)
	echo "backup factory appliction ..."
	cd /rbctrl
	/etc/init.d/rbctrl.sh stop
	sleep 1
	export OPERATE_TYPE=backupFactoryApplication
	./upgrader -plugin rbteach -plugin tslib &
	;;
  *)
	echo "Usage: $0 {backupPLC|backupJBI|backupDatabase|backupFactoryApplication}"
	;;
esac


#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
upgradePath="/update"
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
	cd $upgradePath
	killall -9 cobot
	./upgrader backupDatabase -plugin rbteach -plugin tslib &
	cd ..
	;;
  backupFactoryApplication)
	echo "backup factory appliction ..."
	cd $upgradePath
	/etc/init.d/rbctrl.sh stop
	./upgrader backupFactoryApplication -plugin rbteach -plugin tslib &
	;;
  *)
	echo "Usage: $0 {backupPLC|backupJBI|backupDatabase|backupFactoryApplication}"
	;;
esac


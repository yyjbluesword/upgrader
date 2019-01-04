#!/bin/sh
plcSysPath="/rbctrl/data/sys"
plcUDiskPath="/mnt/udisk/rbctrl/data/sys"
jbiSysPath="/rbctrl/program"
jbiUDiskPath="/mnt/udisk/rbctrl/program"
upgradePath="/update"
case "$1" in
  backupPLC)
 	echo "backup plc data ..."
	if [ -d $plcUDiskPath ]; then
    echo "the dir have been created!";
	else
    mkdir $plcUDiskPath -p
	fi
	cp ${plcSysPath}/plc.dat $plcUDiskPath
	;;
  backupJBI)
	echo "backup jbi files ..."
	if [ -d $jbiUDiskPath ]; then
    echo "the dir have been created!";
	else
    mkdir $jbiUDiskPath -p
	fi
	cp ${jbiSysPath}/* $jbiUDiskPath -r
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


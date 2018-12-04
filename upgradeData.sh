#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
kernelPath="/mnt/udisk/updata/"
upgraderPath="/update"
case "$1" in
  upgradeKernel)
	cd $upgraderPath
	echo "prepare to upgrade kernel"
	/etc/init.d/rbctrl.sh stop
	./upgrader $1 -plugin rbteach -plugin tslib &
	;;
  upgradeServo)
#	echo "backup jbi files ..."
#	if [ -d $jbiPath ]; then
#   echo "the dir have been created!";
#	else
#    mkdir $jbiPath -p
#	fi
#	cp /rbctrl/*.jbi $jbiPath
	;;
  upgradeDatabase)
	cd $upgraderPath
	echo "prepare to upgrade Database"
	cp /mnt/udisk/rbctrl/db/elibotDB.upgrade.pkg /update
	/etc/init.d/rbctrl.sh stop
	./upgrader $1 -plugin rbteach -plugin tslib &
	;;
  upgradeApplication)
	cd $upgraderPath
	echo "Prepare to upgrade Application"
	/etc/init.d/rbctrl.sh stop
	./upgradeApplication.sh /mnt/udisk
	/var/volatile/update/chrt-sqfs.sh /update/upgrader $1
	;;
  *)
	echo "Usage: $0 {upgradeKernel|upgradeServo|upgradeDatabase|upgradeApplication}"
	;;
esac


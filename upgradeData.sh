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
  upgradeServo_[1-7])
	echo "prepare to upgrade servo"
	/etc/init.d/rbctrl.sh stop
	./upgrader $1 -plugin rbteach -plugin tslib &
	;;
  upgradeDatabase)
	cd $upgraderPath
	echo "prepare to upgrade Database"
	cp /mnt/udisk/rbctrl/update/elibotDB.upgrade.pkg /update
	/etc/init.d/rbctrl.sh stop
	./upgrader $1 -plugin rbteach -plugin tslib &
	;;
  upgradeApplication)
	cd $upgraderPath
	echo "Prepare to upgrade Application"
	./upgrader $1 -plugin rbteach -plugin tslib &
	;;
  *)
	echo "Usage: $0 {upgradeKernel|upgradeServo|upgradeDatabase|upgradeApplication}"
	;;
esac


#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
kernelPath="/mnt/udisk/updata/"
case "$1" in
  upgradeKernel)
	cd /rbctrl
	echo "prepare to upgrade kernel"
	/etc/init.d/rbctrl.sh stop
	export OPERATE_TYPE=upgradeKernel
	export OPERATE_TYPE
	./upgrader -plugin rbteach -plugin tslib &
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
	cd /rbctrl
	echo "prepare to upgrade Database"
	cp /mnt/udisk/rbctrl/db/elibotDB.upgrade.pkg /update
	/etc/init.d/rbctrl.sh stop
	export OPERATE_TYPE=upgradeDatabase
	export OPERATE_TYPE
	./upgrader -plugin rbteach -plugin tslib &
	;;
  upgradeApplication)
#	echo "backupFactoryApplication..."
#	/etc/init.d/rbctrl.sh stop
#	cd /
#	tar -xvf rbctrl.tar /rbctrl
#	echo "backupFactoryApplication finished."
	;;
  *)
	echo "Usage: $0 {upgradeKernel|upgradeServo|upgradeDatabase|upgradeApplication}"
	;;
esac


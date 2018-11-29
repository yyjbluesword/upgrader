#!/bin/sh
plcPath="/mnt/udisk/rbctrl/data/user/"
jbiPath="/mnt/udisk/rbctrl/program/"
case "$1" in
  upgradeKernel)
# 	echo "backup plc data ..."
#	if [ -d $plcPath ]; then
#    echo "the dir have been created!";
#	else
#    mkdir $plcPath -p
#	fi
#	cp /rbctrl/plc.dat $plcPath
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
#	echo "backupDatabase..."
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


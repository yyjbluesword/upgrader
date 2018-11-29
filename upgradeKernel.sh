#!/bin/sh

export OPERATE_STATUS=OPERATING
export OPERATE_STATUS
sleep 10
cd /update
echo "prepare to upgrade kernel"
cp /mnt/udisk/rbctrl/update/upgradeKernel.eku .
chmod 777 upgradeKernel.eku
sync
sleep 2
./upgradeKernel.eku
sleep 2
rm upgradeKernel.eku
export OPERATE_STATUS=OPERATEOVER
export OPERATE_STATUS
echo "upgrade kernel successed"


#!/bin/sh

echo "start to recovery Factory Application"
export OPERATE_STATUS=OPERATEING
export OPERATE_STATUS
mv /rbctrl /tmp
cd /
tar -xvf rbctrl.tar /
sleep 3
rm -rf /tmp/rbctrl
sync
export OPERATE_STATUS=OPERATEOVER
export OPERATE_STATUS
echo "recovery Factory Application finished."


#!/bin/sh

echo "start to back up Factory Application"
export OPERATE_STATUS=OPERATEING
export OPERATE_STATUS
cd /
tar -cvf rbctrl.tar /rbctrl
sleep 3
export OPERATE_STATUS=OPERATEOVER
export OPERATE_STATUS
echo "back up Factory Application finished."


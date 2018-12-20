#!/bin/sh

echo "Start to back up Factory Application"
cd /
tar -cvf rbctrl.tar /rbctrl
if [ $? -ne 0 ]; then
	echo "Backup Factory Application Failed!"
	echo "failed"
	exit 1
fi
sync
echo "Backup Factory Application success."
echo "success"


#!/bin/sh

prepareRecovery()
{	
	mv /rbctrl /tmp
	if [ $? -ne 0 ]; then
	echo "prepare recovery Facotry Application failed"
	echo "failed"
	exit 1
	fi
}
decompressingFiles()
{
	cd /
	tar -xvf rbctrl.tar
	if [ $? -ne 0 ]; then
	echo "Decompressing files failed"
	echo "failed"
	exit 2
	fi
}
removeData()
{
	rm -rf /tmp/rbctrl
	if [ $? -ne 0 ]; then
	echo "Remove data failed"
	echo "failed"
	exit 3
	fi
}

echo "start to recovery Factory Application"
prepareRecovery
decompressingFiles
removeData
echo "recovery Factory Application success."
echo "success"



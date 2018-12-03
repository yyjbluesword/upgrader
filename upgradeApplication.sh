#!/bin/sh
#usage: prepare-update.sh <path of firmware.bin>

set -o nounset -o errexit

TMPUPDATE=/var/volatile/update

mkdir -p $TMPUPDATE
mount -o loop -t squashfs $1/firmware.bin $TMPUPDATE
exec $TMPUPDATE/prepare-update.sh $TMPUPDATE/update_rootfs.sqfs


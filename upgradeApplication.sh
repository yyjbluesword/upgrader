#!/bin/sh
set -o nounset -o errexit
echo "Prepare to upgrade Application"
cp /mnt/udisk/rbctrl/update/updateApplication.eau .
sync
echo "Ready to upgrade Application"
./updateApplication.eau
echo "Prepare to delete temp file"
rm updateApplication.eau
sync
echo "Upgrade Application success"
echo "success"


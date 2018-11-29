#!/bin/sh

cd /rbctrl
killall -9 upgrader
./cobot -plugin rbteach -plugin tslib &


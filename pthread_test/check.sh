#!/bin/sh
PROC_NAME=manager
ProcNumber='ps | grep -w $PROC_NAME | grep -v grep | wc -l'
if [ $ProcNumber -le 0]; then
    echo "manager is not run"
    manager &
else
    echo "manager is running"
fi
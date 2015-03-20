#!/bin/sh

ELEVATE="./elevate"

PERMITTED_DOMAINS="`$ELEVATE list`"

if [ -z "$1" ]; then
	ACTIVE_DOMAINS="$PERMITTED_DOMAINS"
else
	ACTIVE_DOMAINS="$1"
fi

TMP=""
NUM=0
for i in $ACTIVE_DOMAINS; do
	NUM=$(($NUM+1))
	TMP="$TMP $i $i"
done
if [ "$NUM" -gt "1" ]; then
	DOMAIN=`whiptail --backtitle "Xen Rescue Manager" --menu "Choose a domain to rescue" 0 0 5 $TMP 3>&1 1>&2 2>&3`
		if [ "$?" != "0" ]; then
			echo
			exit
		fi
else
	DOMAIN="$ACTIVE_DOMAINS"
fi

while :; do
	$ELEVATE domid $DOMAIN > /dev/null 2>&1
	if [ "$?" = "0" ]; then
		RUNNING=1
		ACTION=`whiptail --backtitle "Xen Rescue Manager" --menu "Rescue manager: $DOMAIN" 0 0 10 console "Attach the console" reboot "Trigger a reboot" shutdown "Trigger a shutdown" reset "Hard-reset the domain" destroy "Kill the domain" 3>&1 1>&2 2>&3`
		if [ "$?" != "0" ]; then
			break
		fi
	else
		RUNNING=0
		echo "TBI"
		break
	fi
	case "$ACTION" in
		console)
			echo "Attaching Xen console"
			$ELEVATE $ACTION $DOMAIN
			reset
			echo "Detached Xen console"
			;;
		*)
			ACTION=info
			$ELEVATE $ACTION $DOMAIN
			;;
	esac
	echo "Press enter to continue"
	read X
done

echo

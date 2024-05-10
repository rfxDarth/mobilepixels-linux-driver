#!/bin/sh
# Start/stop the evdi_t6 daemon.
#
### BEGIN INIT INFO
# Provides:          evdi_t6
# Required-Start:    $remote_fs $syslog $time
# Required-Stop:     $remote_fs $syslog $time
# Should-Start:      $network $named slapd autofs ypbind nscd nslcd winbind
# Should-Stop:       $network $named slapd autofs ypbind nscd nslcd winbind
# Default-Start:     2 3 4 5
# Default-Stop:
# Short-Description: Regular background program processing daemon
# Description:       evdi_t6 is a standard UNIX program that runs user-specified 
#                    programs at periodic scheduled times. vixie cron adds a 
#                    number of features to the basic UNIX evdi_t6, including better
#                    security and more powerful configuration options.
### END INIT INFO
 

 
SERVICE_NAME=`basename $0`
PIDFILE=/var/run/t6evdi.pid
LOGPATH="/var/log/${SERVICE_NAME}"
FOREVER_BIN=`which forever`
APP_PATH="/usr/sbin/T6evdi"

case $1 in
	start)
		if [ -e "${PIDFILE}" ]; then
			PID=`cat ${PIDFILE}`
			echo "Service is running already. (PID=${PID})"
			exit 1
		fi
		if [ ! -d "${LOGPATH}" ]; then
			mkdir -p "${LOGPATH}"
		fi
		${APP_PATH} &
		PID=`ps aux | grep ${APP_PATH} | head -n1 | awk '{print $2}'`
		
		rm -rf ${PIDFILE}
		echo "${PID}" > ${PIDFILE}
		echo "Service ${SERVICE_NAME} start. PID=${PID}"
		;;
	stop)
		if [ ! -e "${PIDFILE}" ]; then
			echo "Service is not running."
		else
			PID=`cat ${PIDFILE}`
			killall -9 T6evdi
			rm -rf ${PIDFILE}
			echo "Service ${SERVICE_NAME} stop. PID=${PID}"
		fi
		;;
	restart)
		$0 stop
		sleep 1
		$0 start
		;;
	status)
		if [ -e "${PIDFILE}" ]; then
			PID=`cat ${PIDFILE}`
			echo "Service is running. (PID=${PID})"
		else
				echo "Service is not running."
		fi
		;;
	*)
		echo "Usage: ${0} {start|stop|restart|status}"
		exit 1
		;;
esac

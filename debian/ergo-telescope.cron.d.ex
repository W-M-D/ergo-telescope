#
# Regular cron jobs for the ergo-telescope package
#
0 4	* * *	root	[ -x /usr/bin/ergo-telescope_maintenance ] && /usr/bin/ergo-telescope_maintenance

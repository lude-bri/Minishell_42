#
##
### GDB Settings
##
#
set prompt (zdb) 
set editing on
# set verbose on
set tui border-kind acs
set tui active-border-mode bold-standout
set tui border-mode reverse
set trace-commands on
set logging enabled on
set follow-fork-mode child
# set detach-on-fork off

show follow-fork-mode

set print pretty on
# set print elements 2
set print array on
#
skip -gfi *libreadline.so.8

#
##
### Custom Commands
##
#
# Valgrind integration
define mchk
	monitor leak_check full reachable any
end

# Custom Commands
define rfr
	refresh
end

define nr
  next
  refresh
end
define xx
	kill
	exit
end
### 000_main.c

#
##
### Go GDB Go! I Choose YOU! 
##
#
### 000_main.c
## main()
break main
run
main
fs cmd
rfr


### Info Stats
info break
info watch

#include <kernel/bin/ps.hpp>
#include <kernel/tty.hpp>
#include <kernel/proc/thread.hpp>

// TODO: create an extendable method for commands
static void ps_p(int proc_id) {
	for(Thread* t = thread_root; t->next != thread_root; t = t->next)
		//if(t->proc_id == proc_id)
		terminal_printf("%d\t%d\t00:00:01\t%s\n",t->proc_id,t->thread_id, t->title);
}

// TODO: create an extendable method for commands
int ps(std::vector<std::string>  args) {
	terminal_writestring("PID\tTID\tTIME\tSTR\n");

	for(int i = 1; i < args.size(); i++) {
		if(strncmp((char*)args[i], "-p", 2) == 0) {
			ps_p(atoi((char*)args[i+1]));
		}
	}

	return 0;
}
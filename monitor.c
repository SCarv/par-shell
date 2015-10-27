#include <time.h> // time()
#include <stdio.h> // perror()
#include <sys/wait.h> // wait()
#include <semaphore.h> 

#include "main.h" // exit_called, main_mutex, children_list, waited_children, children_count
#include "monitor.h" // self


void* monitor(void* _)
{

	time_t endtime;

	int pid, count;

	for(;;)
	{	

		sem_wait(&can_wait); // waits for more children to be forked
	
		sem_getvalue(&can_wait, &count); // gets sem_t can_wait's value into count

		if (atomic_get_exit_called() && !count) break; 
		/* Only breaks if exit has been input AND there are no more children to be waited on. */

		pid = wait(NULL); // waits for next child to terminate

		sem_post(&can_fork); // signals that more children can be forked 

		time(&endtime);
	
		if (endtime == -1) 
			perror("par-shell: [ERROR] couldn't get finish time for child");

		if (pid == -1)
			perror("par-shell: [ERROR] couldn't wait on child");  

		else
		{
			atomic_inc_waited_children();
			atomic_update_terminated_process(pid, endtime);
		}

	}

	return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "par_run.h" // self
#include "main.h" // children_list, children_count, can_fork, can_wait


void par_run(char* argVector[]) 
{
	pthread_mutex_lock(&children_mutex);
				
	while (!fork_slot_avaliable()) 
		pthread_cond_wait(&can_fork, &children_mutex);
	
	pthread_mutex_unlock(&children_mutex);	

	pid_t pid = fork();

	if (pid == -1)  //is parent; if child is created unsucessfully
		perror("par-shell: unable to fork");		

	else if (pid == 0) // is child
	{
		execv(argVector[0], argVector);

		if (argVector[0][0] != '/')		
			execvp(argVector[0], argVector);

		// next line only reached if execv fails.
		perror("par-shell: exec failed");
		exit(EXIT_FAILURE);
	}

	else // is parent; if child is created successfully
	{
		atomic_insert_new_process(pid, time(NULL));		
		atomic_inc_forked_children();
		pthread_cond_signal(&can_wait);
	}
	
}


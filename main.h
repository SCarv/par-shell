#ifndef PARSHELL_MAIN_H
#define PARSHELL_MAIN_H

#include <stdbool.h>


extern pthread_cond_t can_fork;
extern pthread_cond_t can_wait;

/** WARNING:
 *  mutex below must NEVER be locked before
 *	calling these functions: */
inline void atomic_inc_waited_children(void);
inline void atomic_inc_forked_children(void);
// end WARNING

extern pthread_mutex_t children_mutex; // XXX: mutex

/** WARNING:
 *  mutex above must ALWAYS be locked before
 *	calling these functions: */
inline bool fork_slot_avaliable(void);
inline bool wait_slot_avaliable(void);
//end WARNING

extern unsigned int iteration_count;
extern time_t total_time;

/* Unlike the ones above, all following functions automatically
 *  guarantee mutual exclusion when called. */

/*new node into list containing info about all children*/
void atomic_insert_new_process(int pid, time_t starttime);

/*update endtime of terminated child with pid 'pid' */
void atomic_update_terminated_process(int pid, time_t endtime); 

bool atomic_get_exit_called(void);

time_t atomic_get_process_time(int pid);


#endif


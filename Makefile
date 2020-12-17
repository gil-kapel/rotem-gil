CC = gcc
PQ_OBJS = queue_priority.o tests_pq.o
EM_OBJS = date.o event.o member.o event_manager.o event_manager_tests.o
PQ_EXEC = queue_priority
EM_EXEC = event_manager
DEBUG_FLAG = DNDEBUG -g
C_FLAG = -std=c99 -Wall -Werror $(DEBUG_FLAG)
PQ_SOURCE = priority_queue.c
EM_SOURCE = date.c event.c member.c event_manager.c priority_queue.c


$(PQ_EXEC): $(PQ_OBJS)
	$(CC) $(DEBUG_FLAG) $(PQ_OBJS) -o $@

$(EM_EXEC): $(EM_OBJS)
	$(CC) $(DEBUG_FLAG) $(EM_OBJS) -o $@

event_manager.o: event_manager.c event_manager.h date.h priority_queue.h event.h member.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c

date.o: date.c date.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c

event.o: event.c event.h date.h priority_queue.h member.h event_manager.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c

member.o: member.c member.h event_manager.h date.h priority_queue.h event.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c
	
pq_tests.o: pq_tests.c test_utilities.h priority_queue.h
	$(CC) $(DEBUG_FLAG) $(C_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)


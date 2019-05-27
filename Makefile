sched: process.cpp main.cpp event.cpp scheduler.cpp eventQueue.cpp
	g++-8.1 -g -o sched process.cpp main.cpp event.cpp scheduler.cpp eventQueue.cpp
#module load gcc-8.1
clean:
	rm -f sched *~

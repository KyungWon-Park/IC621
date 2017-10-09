Odd even transposition sorting program

must be run in mpi environment

Check global_time_report.txt after execution

park0kyung0won@gmail.com

Usage:

	0) Prompted problem set size version

	mpirun -n <num_of_nodes> ./oddeven

	1) For batch job

	mpirun -n <num_of_nodes> ./oddeven <problem_set_size>

	ex)

	mpirun -n 4 ./oddeven 8

	-> Runs oddeven with 4 nodes and problem set of 8 M integer keys

Homework:

	just execute do_hw_batch.sh

	sh ./do_hw_batch.sh

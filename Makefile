# makefile for scheduling program
#
# make rr_p - for round-robin scheduling
# make edf - for edf scheduling

CC=gcc
CFLAGS=-Wall -pthread

clean:
	rm -rf *.o
	rm -rf rr_p
	rm -rf edf

rr_p: Driver.o list.o CPU.o schedule_rr_p.o
	$(CC) $(CFLAGS) -o rr_p Driver.o schedule_rr_p.o list.o CPU.o

edf: Driver.o list.o CPU.o schedule_edf.o timer.o
	$(CC) $(CFLAGS) -o edf Driver.o schedule_edf.o list.o CPU.o timer.o

Driver.o: Driver.c
	$(CC) $(CFLAGS) -c Driver.c

schedule_rr_p.o: schedule_rr_p.c
	$(CC) $(CFLAGS) -c schedule_rr_p.c

schedule_edf.o: schedule_edf.c
	$(CC) $(CFLAGS) -c schedule_edf.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c CPU.h
	$(CC) $(CFLAGS) -c CPU.c

aging_p.o: schedule_aging_p.c timer.o
	$(CC) $(CFLAGS) -c schedule_aging_p.c timer.o

rr: Driver.o list.o CPU.o schedule_rr.o
	$(CC) $(CFLAGS) -o rr Driver.o schedule_rr.o list.o CPU.o

timer.o: timer.c timer.h 
	$(CC) $(CFLAGS) -c timer.c 
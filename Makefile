CC ?= gcc
CFLAGS_common ?= -O0 -Wall -std=gnu99

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c -lpthread

phonebook_opt: $(SRCS_common) smaz.c smaz.h phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) smaz.c smaz.h $@.c -lpthread

run1: $(EXEC)
	watch -d -t ./phonebook_orig
run2: $(EXEC)
	watch -d -t ./phonebook_opt
static1:
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh 
	sudo perf stat -r 10 -e cache-misses -e cache-references -e instructions -e cycles ./phonebook_orig
static2:
	echo "echo 1 > /proc/sys/vm/drop_caches" | sudo sh
	sudo perf stat -r 10 -e cache-misses -e cache-references -e instructions -e cycles ./phonebook_opt
clean:
	$(RM) $(EXEC) *.o perf.*

all:mmap

mmap:
	gcc test_mmap.c -o test_mmap
	
flist:
	gcc test_fl.c ../source/free_list.c -o test_fl
ptr:
	gcc test_ptr.c -o test_ptr
clean:
	rm test_mmap test_fl test_ptr
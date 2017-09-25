

STD=-std=c99
WARN=-Wall
DEBUG=
GPROF=
OPT=-O5
C_FLAGS=$(STD) $(WARN) $(OPT) $(DEBUG)
EXE_NAME=wfs


source: tmmap


# 使用哈希表的程序
hash: wfs_hash.o
	gcc $(GPROF) -o $(EXE_NAME) $<


# 使用字典树的程序
trie: wfs_trie.o
	gcc $(GPROF) -o $(EXE_NAME) $<


# 使用哈希版 mmap 读文件的程序
hmmap: wfs_hash_mmap.o
	gcc $(GPROF) -o $(EXE_NAME) $<

# 使用字典树 mmap 读文件的程序
tmmap: wfs_trie_mmap.o
	gcc $(GPROF) -o $(EXE_NAME) $<


# 测试运行速度的程序
# hash
testh: 
	gcc $(GPROF) -DTABLE_SIZE=8192 -DFILE_NAME='"essay_test.txt"' $(C_FLAGS) -o $(EXE_NAME) wfs_hash.c	

# hash mmap
testhm: 
	gcc $(GPROF) -DTABLE_SIZE=8192 -DFILE_NAME='"essay_test.txt"' $(C_FLAGS) -o $(EXE_NAME) wfs_hash_mmap.c	

# trie mmap
testtm: 
	gcc $(GPROF) -DFILE_NAME='"essay_test.txt"' $(C_FLAGS) -o $(EXE_NAME) wfs_trie_mmap.c	

# trie
testt: 
	gcc $(GPROF) -DFILE_NAME='"essay_test.txt"' $(C_FLAGS) -o $(EXE_NAME) wfs_trie.c	


%.o: %.c
	gcc -c $(C_FLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(EXE_NAME) gmon.out

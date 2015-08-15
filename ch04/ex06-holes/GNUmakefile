PROGS = check-holes copy-holes make-file-with-holes-apue make-file-with-holes
CLEAN_FILES = file.hole

all_and_file_hole: all file.hole


file.hole: make-file-with-holes
	./make-file-with-holes file.hole

include ../../common.mk

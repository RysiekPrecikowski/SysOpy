CC = gcc
CFLAGS = -Wall -std=c11 -g

run: compile
	./main merge_files a:b merge_files aa:bb merge_files aa:a merge_files bb:b

clean:
	rm -f *.o *.a *.so main *.tmp file*.txt

compile:
	make clean
	$(CC) $(CFLAGS) -c myLib.c -o myLib.o
	ar rcs libmyLib.a myLib.o
	$(CC) $(CFLAGS) main.c -l myLib -L ./ -o main

small:
	@echo "\n\nsmall test - 3 files merged\n"
	@for size in 10000 1000000 10000000 ; do \
		echo "********** files of size: $$size ************\n" ; \
		./generator 3 $$size file ; \
		./main merge_files file0.txt:file1.txt \
		merge_files file1.txt:file2.txt \
		merge_files file2.txt:file0.txt ; \
	done

medium:
	@echo "\n\nmedium test - 30 files merged\n"
	@for size in 10000 1000000 10000000 ; do \
  		echo "********** files of size: $$size ************\n" ; \
		./generator 30 $$size file ; \
		./main merge_files file0.txt:file1.txt \
		merge_files file1.txt:file2.txt \
		merge_files file2.txt:file3.txt \
		merge_files file3.txt:file4.txt \
		merge_files file4.txt:file5.txt \
		merge_files file5.txt:file6.txt \
		merge_files file6.txt:file7.txt \
		merge_files file7.txt:file8.txt \
		merge_files file8.txt:file9.txt \
		merge_files file9.txt:file10.txt \
		merge_files file10.txt:file11.txt \
		merge_files file11.txt:file12.txt \
		merge_files file12.txt:file13.txt \
		merge_files file13.txt:file14.txt \
		merge_files file14.txt:file15.txt \
		merge_files file15.txt:file16.txt \
		merge_files file16.txt:file17.txt \
		merge_files file17.txt:file18.txt \
		merge_files file18.txt:file19.txt \
		merge_files file19.txt:file20.txt \
		merge_files file20.txt:file21.txt \
		merge_files file21.txt:file22.txt \
		merge_files file22.txt:file23.txt \
		merge_files file23.txt:file24.txt \
		merge_files file24.txt:file25.txt \
		merge_files file25.txt:file26.txt \
		merge_files file26.txt:file27.txt \
		merge_files file27.txt:file28.txt \
		merge_files file28.txt:file29.txt \
		merge_files file29.txt:file0.txt ; \
	done

large:
	@echo "\n\nlarge test - 100 files merged\n"
	@for size in 1000 100000 4000000 ; do \
  		echo "********** files of size: $$size ************\n" ; \
		./generator 100 $$size file ; \
		./main merge_files file0.txt:file1.txt \
        merge_files file1.txt:file2.txt \
        merge_files file2.txt:file3.txt \
        merge_files file3.txt:file4.txt \
        merge_files file4.txt:file5.txt \
        merge_files file5.txt:file6.txt \
        merge_files file6.txt:file7.txt \
        merge_files file7.txt:file8.txt \
        merge_files file8.txt:file9.txt \
        merge_files file9.txt:file10.txt \
        merge_files file10.txt:file11.txt \
        merge_files file11.txt:file12.txt \
        merge_files file12.txt:file13.txt \
        merge_files file13.txt:file14.txt \
        merge_files file14.txt:file15.txt \
        merge_files file15.txt:file16.txt \
        merge_files file16.txt:file17.txt \
        merge_files file17.txt:file18.txt \
        merge_files file18.txt:file19.txt \
        merge_files file19.txt:file20.txt \
        merge_files file20.txt:file21.txt \
        merge_files file21.txt:file22.txt \
        merge_files file22.txt:file23.txt \
        merge_files file23.txt:file24.txt \
        merge_files file24.txt:file25.txt \
        merge_files file25.txt:file26.txt \
        merge_files file26.txt:file27.txt \
        merge_files file27.txt:file28.txt \
        merge_files file28.txt:file29.txt \
        merge_files file29.txt:file30.txt \
        merge_files file30.txt:file31.txt \
        merge_files file31.txt:file32.txt \
        merge_files file32.txt:file33.txt \
        merge_files file33.txt:file34.txt \
        merge_files file34.txt:file35.txt \
        merge_files file35.txt:file36.txt \
        merge_files file36.txt:file37.txt \
        merge_files file37.txt:file38.txt \
        merge_files file38.txt:file39.txt \
        merge_files file39.txt:file40.txt \
        merge_files file40.txt:file41.txt \
        merge_files file41.txt:file42.txt \
        merge_files file42.txt:file43.txt \
        merge_files file43.txt:file44.txt \
        merge_files file44.txt:file45.txt \
        merge_files file45.txt:file46.txt \
        merge_files file46.txt:file47.txt \
        merge_files file47.txt:file48.txt \
        merge_files file48.txt:file49.txt \
        merge_files file49.txt:file50.txt \
        merge_files file50.txt:file51.txt \
        merge_files file51.txt:file52.txt \
        merge_files file52.txt:file53.txt \
        merge_files file53.txt:file54.txt \
        merge_files file54.txt:file55.txt \
        merge_files file55.txt:file56.txt \
        merge_files file56.txt:file57.txt \
        merge_files file57.txt:file58.txt \
        merge_files file58.txt:file59.txt \
        merge_files file59.txt:file60.txt \
        merge_files file60.txt:file61.txt \
        merge_files file61.txt:file62.txt \
        merge_files file62.txt:file63.txt \
        merge_files file63.txt:file64.txt \
        merge_files file64.txt:file65.txt \
        merge_files file65.txt:file66.txt \
        merge_files file66.txt:file67.txt \
        merge_files file67.txt:file68.txt \
        merge_files file68.txt:file69.txt \
        merge_files file69.txt:file70.txt \
        merge_files file70.txt:file71.txt \
        merge_files file71.txt:file72.txt \
        merge_files file72.txt:file73.txt \
        merge_files file73.txt:file74.txt \
        merge_files file74.txt:file75.txt \
        merge_files file75.txt:file76.txt \
        merge_files file76.txt:file77.txt \
        merge_files file77.txt:file78.txt \
        merge_files file78.txt:file79.txt \
        merge_files file79.txt:file80.txt \
        merge_files file80.txt:file81.txt \
        merge_files file81.txt:file82.txt \
        merge_files file82.txt:file83.txt \
        merge_files file83.txt:file84.txt \
        merge_files file84.txt:file85.txt \
        merge_files file85.txt:file86.txt \
        merge_files file86.txt:file87.txt \
        merge_files file87.txt:file88.txt \
        merge_files file88.txt:file89.txt \
        merge_files file89.txt:file90.txt \
        merge_files file90.txt:file91.txt \
        merge_files file91.txt:file92.txt \
        merge_files file92.txt:file93.txt \
        merge_files file93.txt:file94.txt \
        merge_files file94.txt:file95.txt \
        merge_files file95.txt:file96.txt \
        merge_files file96.txt:file97.txt \
        merge_files file97.txt:file98.txt \
        merge_files file98.txt:file99.txt \
        merge_files file99.txt:file0.txt ; \
	done

runTests: compile
	make small
	make medium
	make large
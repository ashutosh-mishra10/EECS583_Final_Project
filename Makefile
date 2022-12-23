CC = /usr/local/bin/clang-15
INC = -I/usr/local/include
CFLAGS = -Xclang -disable-O0-optnone

OPT= /usr/local/bin/opt

all: matmul.s matmul.preopt.ll

%.s: %.c
	$(CC) -S -emit-llvm $(INC) $(CFLAGS) $< -o $@

%.preopt.ll: %.s
		$(OPT) -S -polly-canonicalize $< -o $@

clean:
	rm -f *.preopt.ll *.s *.ll

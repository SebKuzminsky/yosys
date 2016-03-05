
#GENFILES += frontends/verilog/verilog_parser.tab.cc
#GENFILES += frontends/verilog/verilog_parser.tab.h
#GENFILES += frontends/verilog/verilog_parser.output
#GENFILES += frontends/verilog/verilog_lexer.cc
#
#frontends/verilog/verilog_parser.tab.cc: frontends/verilog/verilog_parser.y
#	$(Q) mkdir -p $(dir $@)
#	$(P) $(BISON) -d -r all -b frontends/verilog/verilog_parser $<
#	$(Q) mv frontends/verilog/verilog_parser.tab.c frontends/verilog/verilog_parser.tab.cc
#
#frontends/verilog/verilog_parser.tab.h: frontends/verilog/verilog_parser.tab.cc
#
#frontends/verilog/verilog_lexer.cc: frontends/verilog/verilog_lexer.l
#	$(Q) mkdir -p $(dir $@)
#	$(P) flex -o frontends/verilog/verilog_lexer.cc $<
#
#OBJS += frontends/verilog/verilog_parser.tab.o
#OBJS += frontends/verilog/verilog_lexer.o
#OBJS += frontends/verilog/preproc.o
#OBJS += frontends/verilog/verilog_frontend.o
#OBJS += frontends/verilog/const2ast.o



# Leave off -Wundef -Wredundant-decls -Wtraditional-conversion until flex
# and bison get cleaned up.  Our code is nearly fine now.
# Use -W for now instead of the more modern -Wextra, since lots of people
# still use older compilers.
# Use the following for development
# WARNS = -Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Wpointer-arith -Wcast-align -Wcast-qual -Wsign-compare -Wwrite-strings -Wstrict-overflow=5 -Wformat=2 -pedantic
#
# simplified version for general use, so as not to look so intimidating
#WARNS = -Wall -W
#
# any of the following three should work:
#STANDARD =
#STANDARD = --std=c89 -D_POSIX_C_SOURCE -D_BSD_SOURCE
#STANDARD = --std=c99 -D_POSIX_C_SOURCE -D_BSD_SOURCE
#
#vhd2vl : lex.yy.c vhd2vl.tab.c
#	$(CC) ${STANDARD} ${WARNS} -O2 -g -o vhd2vl lex.yy.c vhd2vl.tab.c -lfl
#
#vhd2vl.tab.c : vhd2vl.y def.h
#	bison -d -v -t vhd2vl.y
#
#lex.yy.c : vhd2vl.l def.h
#	flex -i vhd2vl.l
#
#clean :
#	rm -f lex.yy.c vhd2vl.tab.c vhd2vl.tab.h vhd2vl.output  vhd2vl


GENFILES += frontends/vhdl/vhdl_parser.cc
GENFILES += frontends/vhdl/vhdl_parser.h
GENFILES += frontends/vhdl/vhdl_parser.output
GENFILES += frontends/vhdl/vhdl_lexer.cc

frontends/vhdl/vhdl_parser.cc frontends/vhdl/vhdl_parser.h: frontends/vhdl/vhdl_parser.y frontends/vhdl/def.h
	$(Q) mkdir -p $(dir $@)
	$(P) $(BISON) -d -v -t -r all -b frontends/vhdl/vhdl_parser $<
	$(Q) mv frontends/vhdl/vhdl_parser.tab.c frontends/vhdl/vhdl_parser.cc
	$(Q) mv frontends/vhdl/vhdl_parser.tab.h frontends/vhdl/vhdl_parser.h

frontends/vhdl/vhdl_lexer.cc: frontends/vhdl/vhdl_lexer.l frontends/vhdl/def.h
	$(Q) mkdir -p $(dir $@)
	$(P) flex -i -o frontends/vhdl/vhdl_lexer.cc $<

OBJS += frontends/vhdl/vhdl_parser.o
OBJS += frontends/vhdl/vhdl_lexer.o
#OBJS += frontends/verilog/preproc.o
#OBJS += frontends/verilog/verilog_frontend.o
#OBJS += frontends/verilog/const2ast.o

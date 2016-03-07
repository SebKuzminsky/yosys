
GENFILES += frontends/vhdl/vhdl_parser.tab.cc
GENFILES += frontends/vhdl/vhdl_parser.tab.h
GENFILES += frontends/vhdl/vhdl_parser.output
GENFILES += frontends/vhdl/vhdl_lexer.cc

frontends/vhdl/vhdl_parser.tab.cc frontends/vhdl/vhdl_parser.tab.h: frontends/vhdl/vhdl_parser.y frontends/vhdl/def.h
	$(Q) mkdir -p $(dir $@)
	$(P) $(BISON) -d -r all -b frontends/vhdl/vhdl_parser $<
	$(Q) mv frontends/vhdl/vhdl_parser.tab.c frontends/vhdl/vhdl_parser.tab.cc

frontends/vhdl/vhdl_lexer.cc: frontends/vhdl/vhdl_lexer.l frontends/vhdl/def.h frontends/vhdl/vhdl_parser.tab.h
	$(P) flex -o $@ $<

OBJS += frontends/vhdl/vhdl_parser.tab.o
OBJS += frontends/vhdl/vhdl_lexer.o
OBJS += frontends/vhdl/vhdl_frontend.o
OBJS += frontends/vhdl/preproc.o

#OBJS += frontends/verilog/const2ast.o

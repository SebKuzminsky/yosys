/*
 *  yosys -- Yosys Open SYnthesis Suite
 *
 *  Copyright (C) 2012  Clifford Wolf <clifford@clifford.at>
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "kernel/register.h"
#include "kernel/celltypes.h"
#include "kernel/rtlil.h"
#include "kernel/log.h"

USING_YOSYS_NAMESPACE
PRIVATE_NAMESPACE_BEGIN

bool check_label(bool &active, std::string run_from, std::string run_to, std::string label)
{
	if (!run_from.empty() && run_from == run_to) {
		active = (label == run_from);
	} else {
		if (label == run_from)
			active = true;
		if (label == run_to)
			active = false;
	}
	return active;
}

struct PrepPass : public Pass {
	PrepPass() : Pass("prep", "generic synthesis script") { }
	virtual void help()
	{
		//   |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
		log("\n");
		log("    prep [options]\n");
		log("\n");
		log("This command runs a conservative RTL synthesis. A typical application for this\n");
		log("is the preparation stage of a verification flow. This command does not operate\n");
		log("on partly selected designs.\n");
		log("\n");
		log("    -top <module>\n");
		log("        use the specified module as top module (default='top')\n");
		log("\n");
		log("    -nordff\n");
		log("        passed to 'memory_dff'. prohibits merging of FFs into memory read ports\n");
		log("\n");
		log("    -run <from_label>[:<to_label>]\n");
		log("        only run the commands between the labels (see below). an empty\n");
		log("        from label is synonymous to 'begin', and empty to label is\n");
		log("        synonymous to the end of the command list.\n");
		log("\n");
		log("\n");
		log("The following commands are executed by this synthesis command:\n");
		log("\n");
		log("    begin:\n");
		log("        hierarchy -check [-top <top>]\n");
		log("\n");
		log("    prep:\n");
		log("        proc\n");
		log("        opt_const\n");
		log("        opt_clean\n");
		log("        check\n");
		log("        opt -keepdc\n");
		log("        wreduce\n");
		log("        memory_dff [-nordff]\n");
		log("        opt_clean\n");
		log("        memory_collect\n");
		log("        opt -keepdc -fast\n");
		log("\n");
		log("    check:\n");
		log("        stat\n");
		log("        check\n");
		log("\n");
	}
	virtual void execute(std::vector<std::string> args, RTLIL::Design *design)
	{
		std::string top_module, memory_opts;
		std::string run_from, run_to;

		size_t argidx;
		for (argidx = 1; argidx < args.size(); argidx++)
		{
			if (args[argidx] == "-top" && argidx+1 < args.size()) {
				top_module = args[++argidx];
				continue;
			}
			if (args[argidx] == "-run" && argidx+1 < args.size()) {
				size_t pos = args[argidx+1].find(':');
				if (pos == std::string::npos) {
					run_from = args[++argidx];
					run_to = args[argidx];
				} else {
					run_from = args[++argidx].substr(0, pos);
					run_to = args[argidx].substr(pos+1);
				}
				continue;
			}
			if (args[argidx] == "-nordff") {
				memory_opts += " -nordff";
				continue;
			}
			break;
		}
		extra_args(args, argidx, design);

		if (!design->full_selection())
			log_cmd_error("This comannd only operates on fully selected designs!\n");

		bool active = run_from.empty();

		log_header("Executing PREP pass.\n");
		log_push();

		if (check_label(active, run_from, run_to, "begin"))
		{
			if (top_module.empty())
				Pass::call(design, stringf("hierarchy -check"));
			else
				Pass::call(design, stringf("hierarchy -check -top %s", top_module.c_str()));
		}

		if (check_label(active, run_from, run_to, "coarse"))
		{
			Pass::call(design, "proc");
			Pass::call(design, "opt_const");
			Pass::call(design, "opt_clean");
			Pass::call(design, "check");
			Pass::call(design, "opt -keepdc");
			Pass::call(design, "wreduce");
			Pass::call(design, "memory_dff" + memory_opts);
			Pass::call(design, "opt_clean");
			Pass::call(design, "memory_collect");
			Pass::call(design, "opt -keepdc -fast");
		}

		if (check_label(active, run_from, run_to, "check"))
		{
			Pass::call(design, "stat");
			Pass::call(design, "check");
		}

		log_pop();
	}
} PrepPass;

PRIVATE_NAMESPACE_END

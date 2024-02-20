all: gui_demo ppc_pptrc

.PHONY: all clean

gui_demo: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/GUI_DEMO

ppc_pptrc: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/PPC_PPTRC

clean:
		$(MAKE) -C framework clean
		$(MAKE) -C applications/GUI_DEMO clean
		$(MAKE) -C applications/PPC_PPTRC clean

run_gui_demo: gui_demo
	@$(MAKE) -C applications/GUI_DEMO run

run_ppc_pptrc: ppc_pptrc
	@$(MAKE) -C applications/PPC_PPTRC run

run: run_ppc_pptrc

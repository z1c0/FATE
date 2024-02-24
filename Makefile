all: gui_demo ppc_pptrc remote_console

.PHONY: all clean

gui_demo: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/GUI_DEMO

ppc_pptrc: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/PPC_PPTRC

remote_console: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/RemoteConsole

clean:
		$(MAKE) -C framework clean
		$(MAKE) -C applications/GUI_DEMO clean
		$(MAKE) -C applications/PPC_PPTRC clean
		$(MAKE) -C applications/RemoteConsole clean

run_gui_demo: gui_demo
	@$(MAKE) -C applications/GUI_DEMO run

run_ppc_pptrc: ppc_pptrc
	@$(MAKE) -C applications/PPC_PPTRC run

run_remote_console: ppc_pptrc
	@$(MAKE) -C applications/RemoteConsole run

run: run_remote_console

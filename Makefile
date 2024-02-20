all: gui_demo

.PHONY: all clean

gui_demo: framework
		$(MAKE) -C framework
		$(MAKE) -C applications/GUI_DEMO

clean:
		$(MAKE) -C framework clean
		$(MAKE) -C applications/GUI_DEMO clean

run_gui_demo: gui_demo
	@$(MAKE) -C applications/GUI_DEMO run

run: run_gui_demo

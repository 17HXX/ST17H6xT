.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ HID - BuildSet ]----------"

Project_Build:
	@make -r -f HID.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ HID - BuildSet ]----------"


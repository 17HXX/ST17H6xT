.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ gpio - BuildSet ]----------"

Project_Build:
	@make -r -f gpio.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ gpio - BuildSet ]----------"


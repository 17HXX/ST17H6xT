.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ timer - BuildSet ]----------"

Project_Build:
	@make -r -f timer.mk -j 16 -C  ./ 


clean:
	@echo "----------Cleaning project:[ timer - BuildSet ]----------"


.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ bugfix - BuildSet ]----------"

Project_Build:
	@make -r -f bugfix.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ bugfix - BuildSet ]----------"


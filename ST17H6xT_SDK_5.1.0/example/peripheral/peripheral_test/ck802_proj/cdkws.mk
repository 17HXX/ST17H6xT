.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ sbp - BuildSet ]----------"

Project_Build:
	@make -r -f sbp.mk -j 16 -C  ./ 


clean:
	@echo "----------Cleaning project:[ sbp - BuildSet ]----------"


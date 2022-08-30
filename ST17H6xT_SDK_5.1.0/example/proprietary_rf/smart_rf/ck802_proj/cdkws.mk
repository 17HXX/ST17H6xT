.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ smart_rf - BuildSet ]----------"

Project_Build:
	@make -r -f smart_rf.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ smart_rf - BuildSet ]----------"


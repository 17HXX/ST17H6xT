.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ peripheral_test - BuildSet ]----------"

Project_Build:
	@make -r -f peripheral_test.mk -j 16 -C  ./ 


clean:
	@echo "----------Cleaning project:[ peripheral_test - BuildSet ]----------"


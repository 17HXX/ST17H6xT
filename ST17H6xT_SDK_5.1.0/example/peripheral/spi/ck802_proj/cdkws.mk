.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ spi - BuildSet ]----------"

Project_Build:
	@make -r -f spi.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ spi - BuildSet ]----------"


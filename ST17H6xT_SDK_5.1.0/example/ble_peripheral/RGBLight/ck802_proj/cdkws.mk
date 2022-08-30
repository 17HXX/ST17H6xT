.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ RGBLight - BuildSet ]----------"

Project_Build:
	@make -r -f RGBLight.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ RGBLight - BuildSet ]----------"


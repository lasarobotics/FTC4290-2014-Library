@echo -------------------------------
@echo Updating version information...
@cd robotc/lib
@del version.ric
@git rev-list HEAD | wc -l >> version.ric
@echo Done!
@echo -------------------------------
@pause
@echo -------------------------------
@echo Updating version information...
@del version.ric
@git rev-list HEAD | wc -l >> version.ric
@echo Done!
@echo -------------------------------
@pause

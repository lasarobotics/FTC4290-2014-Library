@call .\tools\buildgit.bat
@echo -------------------------------
@echo Updating version information...
@echo -------------------------------
@del .\tools\update\version.ric
@git rev-parse HEAD >> version.ric
@git rev-list HEAD | wc -l >> version.ric
@MOVE version.ric .\tools\update\version.ric
@cd .\tools\update
@start UpdateGUI update
@echo Done!
@echo -------------------------------

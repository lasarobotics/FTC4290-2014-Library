@call .\tools\buildgit.bat
@echo -------------------------------
@echo Updating version information...
@echo -------------------------------
@del .\tools\update\version.ric
::ORIGIN SHOULD BE in git:// FORM
@git ls-remote origin | grep refs/heads/master | cut -f 1 >> version.ric
@git rev-list HEAD | wc -l >> version.ric
@MOVE version.ric .\tools\update\version.ric
@cd .\tools\update
@start UpdateGUI update
@echo Done!
@echo -------------------------------
@echo -------------------------------
@echo Updating version information...
@del .\robotc\lib\version.ric
@del .\tools\update\version.ric
@git rev-parse HEAD >> version.ric
@git rev-list HEAD | wc -l >> version.ric
@COPY version.ric .\tools\update\version.ric
@MOVE version.ric .\robotc\lib\version.ric
@cd .\tools\update
@start UpdateGUI update
@echo Done!
@echo -------------------------------
@pause
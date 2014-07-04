@echo -------------------------------
@echo Updating version information...
@del .\robotc\lib\version.ric
@git rev-list HEAD | wc -l >> version.ric
@MOVE version.ric .\robotc\lib\version.ric
@echo Done!
@echo -------------------------------
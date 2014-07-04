@echo -------------------------------
@echo Updating version information...
<<<<<<< HEAD
@del .\robotc\lib\version.ric
=======
@cd robotc/lib
@del version.ric
@git rev-parse HEAD >> version.ric
>>>>>>> 80186a3608980a17db4abb36bfa1524165e3c2fc
@git rev-list HEAD | wc -l >> version.ric
@MOVE version.ric .\robotc\lib\version.ric
@echo Done!
@echo -------------------------------

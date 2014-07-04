@echo -------------------------------
@echo Building git hooks....
@copy .\tools\gitsetup\pre-commit .\.git\hooks\
@copy .\tools\gitsetup\post-commit .\.git\hooks\ 
@echo Hooks built!
@echo -------------------------------
@pause
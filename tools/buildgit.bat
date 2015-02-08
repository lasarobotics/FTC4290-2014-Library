@echo -------------------------------
@echo Building git hooks....
@echo -------------------------------
@copy .\gitsetup\pre-commit ..\.git\hooks\
@copy .\gitsetup\post-commit ..\.git\hooks\
@echo Hooks built!
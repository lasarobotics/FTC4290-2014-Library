#!/bin/sh

#Checks if version.ric needs to be updated.
#If version.ric matches this, then all is OK.
git rev-parse HEAD~1 >> version.ric
git rev-list HEAD | wc -l >> version.ric

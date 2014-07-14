#!/bin/sh

#Updates version.ric if file generated in updatecheck.sh fails to match.
git ls-remote git://github.com/lasarobotics/ftclibs.git | grep refs/heads/master | cut -f 1 >> version.ric
git rev-list HEAD | wc -l >> version.ric

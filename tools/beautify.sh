#/bin/bash
#Script to enforce rapidsvn coding styles

INDENT="indent --indent-level2 --braces-after-if-line --brace-indent0 --no-blank-lines-after-commas -nut --declaration-indentation1"
FILE=$1
if test ! -f $FILE ; then
  echo "Invalid file $FILE" >&2
else
  mv $FILE $FILE.orig
  #remove tabs
  tr "\t" " " < $FILE.orig > $FILE
  #no for the style
  $INDENT $FILE
fi


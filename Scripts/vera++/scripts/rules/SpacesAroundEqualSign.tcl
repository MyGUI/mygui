# Don't write "val=0" and use spaces around '='

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {

        if [regexp {\S=\S} $line] {
            report $f $lineNumber "use spaces around '='"
        }

        incr lineNumber
    }
}

# Don't use space characters

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {

        if [regexp {^ } $line] {
            report $f $lineNumber "leading spaces used instead tabs"
        }

        incr lineNumber
    }
}

# Don't use space characters

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {

        if [regexp {^\t* } $line] {
            report $f $lineNumber "leading spaces used instead of tabs"
        }

        incr lineNumber
    }
}

# Don't use space characters before '*' in pointers declaration
#if [regexp {^[^=\*]*\S+\w+\s\*\s\w+.*$} $line] {

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {

        if [regexp {^[^=\*]*\S+\w+\s\*\s\w+(\);|\s=\s|,\s).*$} $line] {
            report $f $lineNumber "space character before '*' in pointer declaration"
            report $f $lineNumber $line
        }

        incr lineNumber
    }
}

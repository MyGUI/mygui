# Don't write "val=0" and use spaces around '='

foreach f [getSourceFileNames] {
    set lineNumber 1
    foreach line [getAllLines $f] {

        if [regexp {(\S=\S|operator=\(|operator*=\(|\'=\'|"=|=")} $line] {
            if [regexp {(operator=\(|operator.=\(|\'=\'|"=|="|=\\)} $line] {
            } else {
                report $f $lineNumber "use spaces around '='"
            }
        }

        incr lineNumber
    }
}

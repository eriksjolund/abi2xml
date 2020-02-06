This program parses the binary file format coming from a
ABI PRISM TM 377 DNA Sequencer and writes the information in
plain text to an xml file.

## ABI file format

The ABI file binary format is described in this publication:

[_Raw Data File Formats,
and the Digital and Analog Raw Data Streams
of the ABI PRISM TM 377 DNA Sequencer_](http://www.cs.cmu.edu/afs/cs/project/genome/WWW/Papers/clark.html)

by Clark Tibbetts, Ph. D. Professor of Microbiology, Vanderbilt University, August 1995.

## Command line usage

Usage: `abi2xml -i binaryfile -o xmlfile`

Other option flags are also available. To list them type `abi2xml --help`.

An xslt script can be useful when you want to retrieve information from the xml file. 
Take a look in the xslt_examples sub directory. There you find some example scripts.

You run a xslt script like this:
`xsltproc xsltscript abi2xml-generated-xmlfile`

( xsltproc is a command line utility from the package libxslt )




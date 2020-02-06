Command-line tool for converting the ABIF file format to XML.

## Introduction

The first version of the command-line tool __abi2xml__ was written in 2003 by Erik Sjölund then working at [Karolinska Institutet](https://ki.se/en). It was meant to serve
the purpose of parsing the binary file format coming from an
__ABI PRISM TM 377 DNA Sequencer__ and writing the information as plain text in to an XML file.

In February 2020 the source code was migrated from the [original Sourceforge repository](https://sourceforge.net/projects/abi2xml/)
in to [Github](https://github.com/eriksjolund/abi2xml/). The migrated source code came from the file _abi2xml-1.2.tar.gz_
that was released in 2006.

## ABIF file format

The ABIF file format is described in this publication:

[_Raw Data File Formats,
and the Digital and Analog Raw Data Streams
of the ABI PRISM TM 377 DNA Sequencer_](http://www.cs.cmu.edu/afs/cs/project/genome/WWW/Papers/clark.html)

by Clark Tibbetts, Ph. D. Professor of Microbiology, Vanderbilt University, August 1995.

[__Applied Biosystems__](https://en.wikipedia.org/wiki/Applied_Biosystems) later published their own specification.
The PDF document [_Applied Biosystems Genetic Analysis Data File Format_](https://github.com/dridk/QAbifReader/blob/master/ABIF_File_Format.pdf) is still available at the [QAbifReader Github repo](https://github.com/dridk/QAbifReader/).

## Installation

### Alternative 1: building from source
The qt library version 4 (4.1 or higher) required for building abi2xml. The qt library is available for Linux, Microsoft Windows and Mac OS X and others.

After qt has been installed, unpack the abi2xml sources and run `qmake && make && make install`

### Alternative 2: installing prebuilt binary

If you are running Windows on a 32-bit (i386) platform you can download the file abi2xml-1.2.zip and unzip it. If your Windows computer lacks zip support you first have to install 7-zip to be able to unzip the file.

## Command line usage

Usage: `abi2xml -i binaryfile -o xmlfile`

Other option flags are also available. To list them type `abi2xml --help`.

```
[erik@linux]$ abi2xml --help
abi2xml 1.2

This program parses the binary file format coming from an
ABI PRISM TM 377 DNA Sequencer and writes the information out as
an xml file

Usage: abi2xml [OPTIONS]...

  -h, --help                    Print help and exit
  -V, --version                 Print version and exit
  -i, --input-file=STRING       input abi file
  -o, --output-file=STRING      output xml file
  -I, --input-dir=STRING        input dir with abi files
  -O, --output-dir=STRING       output dir
  -s, --abi-file-suffix=STRING  suffix of abi files ( used with --input-dir )
                                  (default=`abi')
  -a, --int-vector-as-attribute write integer vectors inside attributes ( It
                                  makes file size smaller )
  -e, --input-encoding=STRING   input string encoding.  Available encodings
                                  listed at:
                                  http://doc.trolltech.com/3.0/qtextcodec.html
                                  (default=`Apple Roman')
```


To convert a whole directory of ABI files to XML

```
[erik@linux]$ abi2xml -I dir_with_abi_files -O output_dir
```

If you want to test abi2xml but you don't have any ABI files, you may use the file _staden-src-1-6-0/userdata/Sample_671.ab1_ found in the _staden-src-1-6-0.tar.gz_ from the Staden project.

An XSLT script can be useful when you want to retrieve information from the XML file. 
Take a look in the [xslt_examples](xslt_examples) sub directory. There you find some example scripts.

You run a XSLT script like this:
`xsltproc xsltscript abi2xml-generated-xmlfile`

## Related projects

### QAbifReader

[QAbifReader](https://github.com/dridk/QAbifReader/), Qt5 ABIF file reader for Genetic Analysis. License: GPL v2. Programming language: C++.

### ABIParser.py

[ABIParser.py](https://www.bioinformatics.org/wiki/ABIParser.py) is a python module for parsing ABI files. License: GPL v2. Programming language: Python.

###  Biojava - Class ABITrace
[ABITrace java class](https://biojava.org/docs/api/org/biojava/nbio/core/sequence/io/ABITrace.html) in [Biojava](https://biojava.org/). License: LGPL. Programming language: Java.

### Bioperl - Bio::SeqIO::abi
Bio::SeqIO::abi is a perl module in [Bioperl](https://bioperl.org/) for parsing ABI files. It doesn't actually parse the abi files but uses the Staden package for that ( see Section 5.5, “Staden” ). License: "You may distribute this module under the same terms as perl itself". Programming language: Perl.

### Emboss - abiview
[Emboss](https://en.wikipedia.org/wiki/EMBOSS) includes abiview, an application that parses an abi file and converts the information to vector or bitmap images or to text files. License: GPL. Programming language: C.

### Staden
[Staden](https://en.wikipedia.org/wiki/Staden_Package) has capabilities to extract information from abi trace files ( e.g. the program extract_seq ). License: BSD. Programming language: C.

## References to abi2xml in scientific literature

abi2xml was referenced

* in the book
[_Plant DNA Barcoding and Phylogenetics_](https://www.amazon.com/Plant-DNA-Barcoding-Phylogenetics-Ajmal/dp/365928095X)
that is also available as a [PDF](https://www.researchgate.net/profile/Mohammad_Ajmal_Ali/publication/310234163_Plant_DNA_Barcoding_and_Molecular_Phylogeny/links/582a649d08ae138f1bf34c2c/Plant-DNA-Barcoding-and-Molecular-Phylogeny.pdf).

* in the scientific paper [_Modification of orthogonal tRNAs: unexpected consequences for sense codon reassignment_](https://academic.oup.com/nar/article/44/21/10042/2290919) that was published
in _Nucleic Acids Reasearch_ in 2016.

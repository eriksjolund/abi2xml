<?xml version="1.0" encoding="UTF-8"?>
<!--
    This file is part of xml2hostconf.

    Copyright 2004 Erik Sjölund

    abi2xml is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    xml2hostconf is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xml2hostconf; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" encoding="UTF-8"/>
  <xsl:template match="/"> 
    <xsl:text>Filename=</xsl:text>    
    <xsl:value-of select="/root/abiFile/@filename"/> 
    <xsl:text> 
</xsl:text> 
  </xsl:template> 
</xsl:stylesheet> 

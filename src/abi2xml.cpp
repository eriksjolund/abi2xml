

/*******************************************************************************
 *                                                                             *
 *   Copyright (C) 2003  Erik Sjolund,  (<html>Erik Sj&ouml;lund</html>)       *
 *                       Center for Genomics and Bioinformatics,               *
 *                       Karolinska Institutet,                                *
 *                       Stockholm,                                            *
 *                       Sweden                                                *
 *                                                                             *
 *  Author: Erik Sjolund                                                       *
 *  Email: erik.sjolund@adivo.se                                               *
 *                                                                             *
 *  This program is free software; you can redistribute it and/or modify       *
 *  it under the terms of the GNU General Public License as published by       *
 *  the Free Software Foundation; either version 2 of the License, or          *
 *  (at your option) any later version.                                        *
 *                                                                             *
 *  This program is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              * 
 *  General Public License for more details.                                   *
 *                                                                             *
 *  You should have received a copy of the GNU General Public License          *
 *  along with this program; if not, write to the Free Software                *
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  *
 *                                                                             *
 *******************************************************************************
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <qdatastream.h>
#include <qiodevice.h>

#include <qregexp.h>
#include <algorithm>

#include "abi2xml.h"
#include "datarecordmaker.h"
#include "datarecord.h"
#include <QByteArray>

Abi2xml::Abi2xml( QDataStream & stream_, QDomElement parseRoot,  Params *params_ ) 
  : stream ( stream_ ), params( params_ )
{
  device = stream.device();
  domDoc = parseRoot.ownerDocument();
  domDataStructs = domDoc.createElement( "dataStructures" );
  parseRoot.appendChild( domDataStructs );
  parseInfo = domDoc.createElement( "parseInfo" );
  parseRoot.appendChild( parseInfo );
}
 Abi2xml::~Abi2xml()
{
}

void Abi2xml::parse()
{
  m_offset_to_ABIF = offset_to_ABIF();
  qint32 num_rec =  num_records();
  qint32 offset_to_first = offset_to_first_record();
  quint16 blk_size = block_size();
  if ( blk_size != 28 )
  {
     std::cerr << "Warning: Blocksize is not 28. Maybe you passed a non-valid indata file." << std::endl;
     exit(1);
  }
  parseInfo.setAttribute("block_size" , blk_size);
  parseInfo.setAttribute("offset_to_first_record",offset_to_first);
  parseInfo.setAttribute("read_num_tagged_file_records",num_rec);  
  for ( int i=0 ; i< num_rec; ++i )
  {
    qlonglong offset =  offset_to_first + i * blk_size;
    device->seek( offset );
    DataRecord * record = DataRecordMaker::newDataRecord( stream, m_offset_to_ABIF, params );
    Q_ASSERT( record );
    if ( record )
    {
      QDomElement elem = domDoc.createElement( record->xmlElementName() );
      domDataStructs.appendChild( elem );
      record->produceDom( elem );
    } else
    {
      exit( 1 );
    }
  }
}

quint16 Abi2xml::block_size()
{
  device->seek( 16 + m_offset_to_ABIF );
  quint16 number;
  stream >> number; 
  return number;
}

qint32 Abi2xml::num_records()
{
  qint32 number = read_int32_at( 18 + m_offset_to_ABIF );
  return number;
}

qint32 Abi2xml::offset_to_first_record()
{
  qint32 number = read_int32_at( 26 + m_offset_to_ABIF );
  return number + m_offset_to_ABIF;
}

qint32 Abi2xml::read_int32_at( qlonglong offset )
{
  device->seek( offset );
  qint32 number;
  stream >> number;
  return number;
}

qint32 Abi2xml::offset_to_ABIF()
{
  qint32 index = 0;
  qint32 size = device->size();
  int len = 5;
  bool found = false;
  while ( ( ! found && ( index <= size - len )) )
  {
    device->seek( index );
    char array[ len ];
    stream.readRawData(&array[0], len );
    array[ len -1 ] = '\0';
    QString str = QString(&array[0]);
    if  ( str == "ABIF" ) {
      found = true;
      break;
    }
    ++index;
  }
  if ( ! found )
  {
    std::cerr << "could not find a qualitive guess of the start position" << std::endl;
    exit(1);  
  }
   return index;
}

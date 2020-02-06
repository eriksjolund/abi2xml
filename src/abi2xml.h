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

#ifndef ABI2XML_H
#define ABI2XML_H

// include files for QT

#include <QIODevice>
#include <QList>
#include <QDomElement>

class QDataStream;
class DataRecord;
struct Params;
     
class Abi2xml
{
public:
  Abi2xml( QDataStream & stream, QDomElement, Params *params );
 ~Abi2xml();
 void parse();
 quint16 block_size();
 qint32 num_records(  );
 qint32 offset_to_first_record();
 qint32 offset_to_ABIF();
protected:
  qint32 read_int32_at( qlonglong offset );
  QIODevice * device;
  QDataStream & stream;
  DataRecord * dataRecord;
  QDomElement  domDataStructs;
  QDomElement parseInfo;
  qint32 m_offset_to_ABIF;
  QDomDocument domDoc;
  Params *params;
};

#endif 


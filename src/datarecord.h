
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

#ifndef DATARECORD_H
#define DATARECORD_H

#include <QVector>
#include <QString>

class QTextCodec;
class QDataStream;

struct Params;

#include <QDomElement>





class DataRecord
{
public:
  DataRecord( QDataStream &, qint32 offset_to_ABIF, Params *params );
  virtual ~DataRecord() {}
  virtual QString xmlElementName() { return QString( tag_name ); }
  void produceDom( QDomElement );
protected:
  QString errorInfo();
  virtual void produceSpecialDom( QDomElement ) =0;
  void sanity_check_value( qint32, qint32, QString );
  void sanity_check( const qint32 * const tag_number_wanted,
                     const qint16 * const data_type_wanted = NULL,
                     const qint16 * const element_length_wanted = NULL,
                     const qint32 * const number_of_elements_wanted = NULL,
                     const qint32 * const record_length_wanted = NULL,
                     const qint32 * const data_record_wanted = NULL,
                     const qint32 * const cryptic_variable_wanted = NULL);   
  qlonglong original_offset;
  char tag_name[5];   
  qint32 tag_number;
  qint16 data_type;
  qint16 element_length;
  qint32 number_of_elements;
  qint32 record_length;
  qint32 data_record;
  qint32 cryptic_variable;
  qlonglong offset_to_data;
  Params * params;
};

class CStringDataRecord : public DataRecord
{
public:
  CStringDataRecord( QDataStream & stream,  qint32 offset_to_ABIF , Params *params, qint16 type = ( qint16 ) 2 );
  void produceSpecialDom( QDomElement );
protected:
  QString str;
};

class GELPRecord : public CStringDataRecord
{
  /* GELP has data type 18 and that would mean it has a pascal string
     if it followed the pattern used elsewhere. The GELP
     Tagged_Data_Record is also described as a pascal string in
     the publication:

     -------------------------------------------

     Raw Data File Formats,
     and the Digital and Analog Raw Data Streams
     of the ABI PRISM TM 377 DNA Sequencer

     A preliminary technical examination

     by

     Clark Tibbetts, Ph. D.
     Professor of Microbiology

     A5217 Medical Center North
     Vanderbilt University
     Nashville, TN 37232-2363

     August 1995
     tibbetts@ctrvax.vanderbilt.edu

     ----------------------------------------------

     But it is a c string if you look at some
     of the ABI files we have. Therefore we have
     to inherit CStringDataRecord with additional
     passing of data_type argument = 18 to
     CStringDataRecord constructor.
  */
public:
  GELPRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params ) : CStringDataRecord( stream, offset_to_ABIF , params, 18 ) {}
};

class PROJRecord : public CStringDataRecord
{
public:
  PROJRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params ) : CStringDataRecord( stream, offset_to_ABIF, params, 19  ) {}
};


class Int8VectorDataRecord : public DataRecord
{
public:
  Int8VectorDataRecord( QDataStream & stream,  qint32 offset_to_ABIF , Params *params );
  void produceSpecialDom( QDomElement );
protected:
  QVector<qint8> dataVector;
};


class Int16VectorDataRecord : public DataRecord
{
public:
  Int16VectorDataRecord( QDataStream & stream,  qint32 offset_to_ABIF, Params *params  );
  void produceSpecialDom( QDomElement );
protected:
  QVector< qint16 > dataVector;
};

class S_N_Record : public Int16VectorDataRecord
{
public:
  S_N_Record( QDataStream & stream,  qint32 offset_to_ABIF, Params *params  ) : Int16VectorDataRecord( stream, offset_to_ABIF, params) {}
  QString xmlElementName();
};


class PascalStringDataRecord : public DataRecord
{
public:
  PascalStringDataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params );
  void produceSpecialDom( QDomElement );
protected:
  QString str;
};

QString getPascalString( QDataStream & stream, qint32 record_len, QString, QTextCodec * );


class RUNDRecord : public DataRecord
{
public:
  RUNDRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params );
  void produceSpecialDom( QDomElement );
protected:
  qint16 year;
  qint8 month;
  qint8 day;
};

class RUNTRecord : public DataRecord
{
public:
  RUNTRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params );
  void produceSpecialDom( QDomElement );
protected:
  /* the record_length is 4 but we have 3 qint8. Is it correct? */
  qint8 hour;
  qint8 minute;
  qint8 second;
};

class SPACRecord : public DataRecord
{
public:
  SPACRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params );
  ~SPACRecord() {}
  void produceSpecialDom( QDomElement );
  QString xmlElementName();
protected:
  float number;
  QString str;
};

class UnknownRecord : public DataRecord
{
 public:
  /* this class is there to represent a datastructure that has not been recognized*/

  UnknownRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params ) : DataRecord( stream, offset_to_ABIF, params ) {}
  void produceSpecialDom( QDomElement );
  QString xmlElementName();
};

#endif

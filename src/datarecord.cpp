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

#include <QString>
#include <QDataStream>
#include <QIODevice>

#include <QTextStream>
#include <QTextCodec>
#include <QDomDocument>
#include <QDomElement>
#include "datarecord.h"
#include "params.h"

DataRecord::DataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params * params_ ) 
{
  params = params_;
  QIODevice * dev = stream.device();
  original_offset = dev->pos();

  tag_name[4]='\0'; /* let the fifth byte already have the null
                         to terminate the string */

  stream.readRawData(&tag_name[0], 4);
  stream
    >> tag_number
    >> data_type
    >> element_length
    >> number_of_elements
    >> record_length;

  qlonglong data_record_offset = dev->pos();                    

  stream >> data_record
         >> cryptic_variable;

  if( record_length <= 4 )
  {
    offset_to_data = data_record_offset;
  }
  else
  {
    offset_to_data =  static_cast<qlonglong> ( data_record + offset_to_ABIF );
  }

  /* the sub classes of DataRecord get a stream with the current position set
     to where the data resides */
  if ( ! dev->seek( offset_to_data ) )
  {
    qFatal("Couldn't seek to a data record in the file.\nThe file doesn't seem to follow the normal trace file layout\n");
  }
}

void DataRecord::sanity_check( const qint32 * const tag_number_wanted,
                               const qint16 * const data_type_wanted,      
                               const qint16 * const element_length_wanted,
                               const qint32 * const number_of_elements_wanted,
                               const qint32 * const record_length_wanted,
                               const qint32 * const data_record_wanted,
                               const qint32 * const cryptic_variable_wanted)
{
  if ( tag_number_wanted )
     sanity_check_value( tag_number, *tag_number_wanted, "tag_number");
  if ( data_type_wanted )
     sanity_check_value(data_type, *data_type_wanted, "data_type"); 
  if ( element_length_wanted )
     sanity_check_value(element_length, *element_length_wanted , "element_length");
  if ( number_of_elements_wanted )
     sanity_check_value( number_of_elements, *number_of_elements_wanted, "number_of_elements");
  if ( record_length_wanted )
     sanity_check_value( record_length, *record_length_wanted, "record_length");
  if ( data_record_wanted )
     sanity_check_value(data_record, *data_record_wanted , "data_record");
  if ( cryptic_variable_wanted )
     sanity_check_value( cryptic_variable, *cryptic_variable_wanted, "cryptic_variable"); 
}

void DataRecord::sanity_check_value( qint32 actual, qint32 correct, QString type )
{
  if ( actual != correct )
  {
    qFatal("Error: %s is %d but %d was expected for the %s",qPrintable(type),actual,correct,qPrintable(errorInfo()));
  } 
}

QString DataRecord::errorInfo()
{
  QString str;
  QTextStream ts( &str, QIODevice::WriteOnly );
  ts << "  Datarecord parsed at offset=" << original_offset  << " with " << endl
     << "  tag_name=" << tag_name << endl 
     << "  tag_number" << tag_number << endl
     << "  data_type=" << data_type  << endl
     << "  element_length=" << element_length  << endl
     << "  number_of_elements=" << number_of_elements  << endl
     << "  record_length=" << record_length  << endl
     << "  data_record=" << data_record  << endl
     << "  cryptic_variable=" << cryptic_variable << endl;
  return str;
}
                 
void DataRecord::produceDom( QDomElement node ) {
  node.setAttribute("tag_number", tag_number );
  QDomDocument doc = node.ownerDocument();
  QDomElement common = doc.createElement("common_data_struct");
  node.appendChild( common );
  common.setAttribute("name", tag_name );
  common.setAttribute("tag_number", tag_number );
  common.setAttribute("data_type", data_type );
  common.setAttribute("element_length" , element_length );
  common.setAttribute("number_of_elements", number_of_elements );
  common.setAttribute("record_length", record_length );
  common.setAttribute("data_record" , data_record );
  common.setAttribute("cryptic_variable", cryptic_variable );
  common.setAttribute("calculated_offset_to_data", static_cast< qint32 >(  offset_to_data ));

  produceSpecialDom( node );
}

CStringDataRecord::CStringDataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_, qint16 data_type_wanted ) 
                  :  DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 element_length_wanted = 1;
  sanity_check( NULL,
                &data_type_wanted,
                &element_length_wanted);
  char array[record_length + 1];
  stream.readRawData(&array[0], record_length );
  array[ record_length ] = '\0'; 
  Q_ASSERT( params->codec );
  str = params->codec->toUnicode( &array[0] );
}

void CStringDataRecord::produceSpecialDom( QDomElement node ) {
  node.setAttribute( "string", str );
}

Int8VectorDataRecord::Int8VectorDataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_ )
                             : DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 data_type_wanted = 13;
  qint16 element_length_wanted = 1;
  sanity_check( NULL,
                &data_type_wanted,
                &element_length_wanted);
  for ( int i = 0; i < number_of_elements ; ++i )
  {
    qint8 value;
    stream >> value;
    dataVector.append( value );
  }
}

void Int8VectorDataRecord::produceSpecialDom( QDomElement elem ) {
  QVector<qint8>::iterator it;
  QDomDocument doc = elem.ownerDocument();

  if ( params->int_vector_as_attribute )
  {
    QString v;
    it = dataVector.begin();
    while ( it != dataVector.end() )
    {   
      QString num;
      num = num.setNum( *it );
      v.append( num );
      it++;
      if ( it != dataVector.end() )
      {
        v.append(" ");
      }
    }
    elem.setAttribute("Int8Vector", v );
  } else
  {
    for( it = dataVector.begin(); it != dataVector.end(); ++it )
    {   
      QDomElement elem2 = doc.createElement("int8");
      elem.appendChild( elem2 );
      elem2.setAttribute("value", *it );
    }
  }
};

Int16VectorDataRecord::Int16VectorDataRecord( QDataStream & stream, qint32 offset_to_ABIF , Params *params_ )
                             : DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 data_type_wanted = 4;
  qint16 element_length_wanted = 2;

  /** There seems to be a bug in the ABI binary file format.
      It is not following the scheme that rest of the elements have.

      Comparing

      name="LANE" tag_number="1" data_type="4" element_length="1" number_of_elements="2" record_length="2"

      with 

      name="LPOS"  tag_number="1"  data_type="4"  element_length="2" number_of_elements="1" record_length="2"

      my guess is that element_length and number_of_elements were accidently swapped.
  **/

  if ( xmlElementName() == "LANE" && tag_number == 1 && data_type == 4 && element_length == 1 
       && number_of_elements == 2 && record_length == 2 )
  {
    number_of_elements = 1;
    element_length = 2;
  }

  sanity_check( NULL,
                  &data_type_wanted,
                  &element_length_wanted);

  for ( int i = 0; i < number_of_elements ; ++i )
  {
    qint16 value;
    stream >> value;
    dataVector.append( value );
  }
};

void Int16VectorDataRecord::produceSpecialDom( QDomElement elem ) {
  QVector< qint16 >::iterator it;
  QDomDocument doc = elem.ownerDocument();

  if ( params->int_vector_as_attribute )
  {
    QString v;
    it = dataVector.begin();
    while ( it != dataVector.end() )
    {   
      QString num;
      num = num.setNum( *it );
      v.append( num );
      it++;
      if ( it != dataVector.end() )
      {
        v.append(" ");
      }
    }
    elem.setAttribute("Int16Vector", v );
  } else 
  {
    for( it = dataVector.begin(); it != dataVector.end(); ++it )
    {   
      QDomElement elem2 = doc.createElement("int16");
      elem.appendChild( elem2 );
      elem2.setAttribute("value", *it );
    }
  }
};

QString S_N_Record::xmlElementName() {
  // If I recall correctly, this was because "S/N%" couldn't be used as an xml element name.
  return QString( "S_divide_N_percent" ); 
}

PascalStringDataRecord::PascalStringDataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_ ) :  DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 data_type_wanted = 18;
  qint16 element_length_wanted = 1;

  sanity_check( NULL,
                &data_type_wanted,
                &element_length_wanted);
  str = getPascalString( stream, record_length, QString( tag_name ), params->codec );
}

void PascalStringDataRecord::produceSpecialDom( QDomElement node ) {
  node.setAttribute( "string", str );
}

QString getPascalString( QDataStream & stream, qint32 record_len, QString tag_name, QTextCodec * codec )
{
  char array[record_len + 1];
  array[ record_len ] = '\0';
  stream.readRawData(&array[0], record_len );
  qint8 length = ( char ) array[0];    /* pascal string has length stored */
  if ( length + 1 != record_len )
  {
    qCritical("The first position of the pascal string doesn't match with recordlength\n( pascal string length %d + 1 != record_length %d\nThe error occured for tag name=\"%s\"",( qint16 ) length,  record_len, qPrintable(tag_name));              
  }
  Q_ASSERT( codec );
  QString str = codec->toUnicode( &array[1] );
  return str;
}

RUNDRecord::RUNDRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_ )
                             : DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 data_type_wanted = 10;
  qint16 element_length_wanted = 4;
  qint32 number_of_elements_wanted = 1;
  qint32 record_length_wanted = 4;
  sanity_check( NULL,
                &data_type_wanted,
                &element_length_wanted,
                &number_of_elements_wanted,
                &record_length_wanted);

  stream >> year >> month >> day;
}

RUNTRecord::RUNTRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_ )
                             : DataRecord( stream, offset_to_ABIF, params_ )
{
  qint16 data_type_wanted = 11;
  qint16 element_length_wanted = 4;
  qint32 number_of_elements_wanted = 1;
  qint32 record_length_wanted = 4;
  sanity_check( NULL,
                &data_type_wanted,
                &element_length_wanted,
                &number_of_elements_wanted,
                &record_length_wanted);

  stream >> hour >> minute >> second;
}

void RUNDRecord::produceSpecialDom( QDomElement node ) {
  node.setAttribute( "year", year );
  node.setAttribute( "month", month );
  node.setAttribute( "day", day );
}

void RUNTRecord::produceSpecialDom( QDomElement node ) {
  node.setAttribute( "hour", hour );
  node.setAttribute( "minute", minute );
  node.setAttribute( "second", second );
}
  
SPACRecord::SPACRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params_ )
                             : DataRecord( stream, offset_to_ABIF, params_ )
{
  /* This Tagged_Data_Record with the tag_name "SPAC"
     is odd. The other data structs all have one single
     data_type. But "SPAC" occurs with both data_type=7
     ( float ) and data_type=18 ( pascal string ). 
     Therefore we have to write this class with 
     "if"-cases. To let them be separated in the xml-output
     we give the pascal string case a new element name - 
     "SPACSTRING".
   */

  if ( data_type == 18   && element_length == 1)
  {
    str = getPascalString( stream, record_length, QString( tag_name), params->codec );
  }
  else
  {
    qint16 data_type_wanted = 7;
    qint16 element_length_wanted = 4;
    qint32 number_of_elements_wanted = 1;
    qint32 record_length_wanted = 4;
    sanity_check( NULL,
                  &data_type_wanted,
                  &element_length_wanted,
                  &number_of_elements_wanted,
                  &record_length_wanted);

    stream >> number;
  }
}

void SPACRecord::produceSpecialDom( QDomElement node ) 
{
  if ( data_type == 18   && element_length == 1)
  {
    node.setAttribute( "string", str );
  }
  else
   node.setAttribute( "float", number );
}
  
QString SPACRecord::xmlElementName()
{
  if ( data_type == 18   && element_length == 1)
    return QString("SPACSTRING");
  else
    return QString("SPAC");
}

void UnknownRecord::produceSpecialDom( QDomElement  ) { return; }
QString UnknownRecord::xmlElementName() { return QString( "Unknown" ); }

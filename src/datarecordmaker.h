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

#ifndef DATARECORDMAKER_H
#define DATARECORDMAKER_H

#include <map>
class QDataStream;
class DataRecord;
struct Params;
/**
  *@author 
  */
class DataRecordMaker {
public:
  static DataRecord* newDataRecord( QDataStream &, qint32 offset, Params *params );
  virtual ~DataRecordMaker();
protected:
  DataRecordMaker( const QString & className );
  typedef std::map<QString,DataRecordMaker*> MakerMap;
  virtual DataRecord* makeDataRecord( QDataStream &, qint32 offset, Params *params ) const = 0;
private:
  static QString getTagName( QDataStream & stream );
  static MakerMap & registry();
};

template <class T, const char * str>
class DataRecordMakerTP : public DataRecordMaker {
public:
  ~DataRecordMakerTP() {}
private:
  DataRecord* makeDataRecord( QDataStream &, qint32 offset, Params *params ) const;
  DataRecordMakerTP( ) : DataRecordMaker( str ) {}
  static const DataRecordMakerTP< T, str > registerThis;
};

template <class T, const char * str>
DataRecord* DataRecordMakerTP<T,str>::makeDataRecord( QDataStream & stream, qint32 offset, Params *params ) const
{
  return new T( stream, offset, params );
}

template <class T, const char * str> const DataRecordMakerTP< T, str > DataRecordMakerTP< T, str >::registerThis; 



#endif

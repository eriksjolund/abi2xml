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


#include <iostream>

#include <qiodevice.h>
#include <qdatastream.h> 
#include <qstring.h>

#include "datarecordmaker.h"
#include "datarecord.h" 

DataRecordMaker::MakerMap & DataRecordMaker::registry() {
  /* from c++ faq lite
  [10.12] How do I prevent the "static initialization order fiasco"?
  We use this because static instances of sub classes of DataRecordMaker
  make use of this map */
  static DataRecordMaker::MakerMap map;
  return map;
}

QString DataRecordMaker::getTagName( QDataStream & stream )
{
  char tag_name[5];
  QIODevice * dev = stream.device();
  qlonglong offset = dev->pos();
  tag_name[4]='\0'; /* let the fifth byte already have the null
                         to terminate the string */
  stream.readRawData(tag_name, 4);

  // rewind, so that the DataRecord constructor also can read the tag_name
  dev->seek( offset );
  return QString( tag_name );    
}

DataRecord* DataRecordMaker::newDataRecord( QDataStream & stream, qint32 offset_to_ABIF, Params *params ) {


  QString tag_name = getTagName(stream);

  DataRecord * rec = NULL;

  MakerMap::iterator iter;

  iter = registry().find( tag_name );

  if (  iter != registry().end() )
  {
    DataRecordMaker* maker = iter->second;
    rec = maker->makeDataRecord( stream, offset_to_ABIF, params );
  }
  else
  {
    rec = new UnknownRecord( stream, offset_to_ABIF, params );
  }
  return rec;
}

DataRecordMaker::DataRecordMaker( const QString & className ){
  registry().insert( std::make_pair(className, this) );
}

DataRecordMaker::~DataRecordMaker(){
}

/*
For explanation on:
How can I pass a "string literal" to a template?
see:
Comeau C++ Template FAQ
http://www.comeaucomputing.com/techtalk/templates/#stringliteral
*/




char AEPt[]="AEPt";
template const DataRecordMakerTP<Int16VectorDataRecord,AEPt> DataRecordMakerTP<Int16VectorDataRecord,AEPt>::registerThis;
char ASPF[]="ASPF";
template const DataRecordMakerTP<Int16VectorDataRecord,ASPF> DataRecordMakerTP<Int16VectorDataRecord,ASPF>::registerThis;
char ASPt[]="ASPt";
template const DataRecordMakerTP<Int16VectorDataRecord,ASPt> DataRecordMakerTP<Int16VectorDataRecord,ASPt>::registerThis;
char AUTO[]="AUTO";
template const DataRecordMakerTP<Int8VectorDataRecord,AUTO> DataRecordMakerTP<Int8VectorDataRecord,AUTO>::registerThis; 
char B1Pt[]="B1Pt";
template const DataRecordMakerTP<Int16VectorDataRecord,B1Pt> DataRecordMakerTP<Int16VectorDataRecord,B1Pt>::registerThis;
char CAGT[]="CAGT";
template const DataRecordMakerTP<Int8VectorDataRecord,CAGT> DataRecordMakerTP<Int8VectorDataRecord,CAGT>::registerThis; 
char CMBF[]="CMBF";
template const DataRecordMakerTP<PascalStringDataRecord,CMBF> DataRecordMakerTP<PascalStringDataRecord,CMBF>::registerThis;
char CMNT[]="CMNT";
template const DataRecordMakerTP<PascalStringDataRecord,CMNT> DataRecordMakerTP<PascalStringDataRecord,CMNT>::registerThis; 
char CTTL[]="CTTL";
template const DataRecordMakerTP<PascalStringDataRecord,CTTL> DataRecordMakerTP<PascalStringDataRecord,CTTL>::registerThis; 
char DATA[]="DATA";
template const DataRecordMakerTP<Int16VectorDataRecord,DATA> DataRecordMakerTP<Int16VectorDataRecord,DATA>::registerThis;
char drty[]="drty";
template const DataRecordMakerTP<Int16VectorDataRecord,drty> DataRecordMakerTP<Int16VectorDataRecord,drty>::registerThis;
char EVNT[]="EVNT";
template const DataRecordMakerTP<PascalStringDataRecord,EVNT> DataRecordMakerTP<PascalStringDataRecord,EVNT>::registerThis; 
char FWO_[]="FWO_";
template const DataRecordMakerTP<CStringDataRecord,FWO_> DataRecordMakerTP<CStringDataRecord,FWO_>::registerThis;
char GELN[]="GELN";
template const DataRecordMakerTP<PascalStringDataRecord,GELN> DataRecordMakerTP<PascalStringDataRecord,GELN>::registerThis;
char GELP[]="GELP";
template const DataRecordMakerTP<GELPRecord,GELP> DataRecordMakerTP<GELPRecord,GELP>::registerThis;
char LANE[]="LANE";
template const DataRecordMakerTP<Int16VectorDataRecord,LANE> DataRecordMakerTP<Int16VectorDataRecord,LANE>::registerThis;
char LIMC[]="LIMC";
template const DataRecordMakerTP<Int16VectorDataRecord,LIMC> DataRecordMakerTP<Int16VectorDataRecord,LIMC>::registerThis;
char LIMS[]="LIMS";
template const DataRecordMakerTP<PascalStringDataRecord,LIMS> DataRecordMakerTP<PascalStringDataRecord,LIMS>::registerThis; 
char LPOS[]="LPOS";
template const DataRecordMakerTP<Int16VectorDataRecord,LPOS> DataRecordMakerTP<Int16VectorDataRecord,LPOS>::registerThis;
char MTRX[]="MTRX";
template const DataRecordMakerTP<Int16VectorDataRecord,MTRX> DataRecordMakerTP<Int16VectorDataRecord,MTRX>::registerThis;
char NAVG[]="NAVG";
template const DataRecordMakerTP<Int16VectorDataRecord,NAVG> DataRecordMakerTP<Int16VectorDataRecord,NAVG>::registerThis;
char NLNE[]="NLNE";
template const DataRecordMakerTP<Int16VectorDataRecord,NLNE> DataRecordMakerTP<Int16VectorDataRecord,NLNE>::registerThis;
char PBAS[]="PBAS";
template const DataRecordMakerTP<CStringDataRecord,PBAS> DataRecordMakerTP<CStringDataRecord,PBAS>::registerThis;
char PDMF[]="PDMF";
template const DataRecordMakerTP<PascalStringDataRecord,PDMF> DataRecordMakerTP<PascalStringDataRecord,PDMF>::registerThis;
char PLOC[]="PLOC";
template const DataRecordMakerTP<Int16VectorDataRecord,PLOC> DataRecordMakerTP<Int16VectorDataRecord,PLOC>::registerThis;
char PPOS[]="PPOS";
template const DataRecordMakerTP<Int16VectorDataRecord,PPOS> DataRecordMakerTP<Int16VectorDataRecord,PPOS>::registerThis;
char PROJ[]="PROJ";
template const DataRecordMakerTP<PROJRecord,PROJ> DataRecordMakerTP<PROJRecord,PROJ>::registerThis; 
char RUND[]="RUND";
template const DataRecordMakerTP<RUNDRecord,RUND> DataRecordMakerTP<RUNDRecord,RUND>::registerThis;
char RUNT[]="RUNT";
template const DataRecordMakerTP<RUNTRecord,RUNT> DataRecordMakerTP<RUNTRecord,RUNT>::registerThis;
char S_N_[]="S/N%";
template const DataRecordMakerTP<S_N_Record,S_N_> DataRecordMakerTP<S_N_Record,S_N_>::registerThis;
char SMOD[]="SMOD";
template const DataRecordMakerTP<CStringDataRecord,SMOD> DataRecordMakerTP<CStringDataRecord,SMOD>::registerThis;
char SMPL[]="SMPL";
template const DataRecordMakerTP<PascalStringDataRecord,SMPL> DataRecordMakerTP<PascalStringDataRecord,SMPL>::registerThis;
char SPAC[]="SPAC";
template const DataRecordMakerTP<SPACRecord,SPAC> DataRecordMakerTP<SPACRecord,SPAC>::registerThis;
char TRKC[]="TRKC";
template const DataRecordMakerTP<Int16VectorDataRecord,TRKC> DataRecordMakerTP<Int16VectorDataRecord,TRKC>::registerThis;
char TRKP[]="TRKP";
template const DataRecordMakerTP<Int16VectorDataRecord,TRKP> DataRecordMakerTP<Int16VectorDataRecord,TRKP>::registerThis;
char MCHN[]="MCHN";
template const DataRecordMakerTP<PascalStringDataRecord,MCHN> DataRecordMakerTP<PascalStringDataRecord,MCHN>::registerThis; 
char MODL[]="MODL";
template const DataRecordMakerTP<CStringDataRecord,MODL> DataRecordMakerTP<CStringDataRecord,MODL>::registerThis; 
char MTXF[]="MTXF";
template const DataRecordMakerTP<PascalStringDataRecord,MTXF> DataRecordMakerTP<PascalStringDataRecord,MTXF>::registerThis; 
char NChn[]="NChn";
template const DataRecordMakerTP<Int16VectorDataRecord,NChn> DataRecordMakerTP<Int16VectorDataRecord,NChn>::registerThis;
char RSPN[]="RSPN";
template const DataRecordMakerTP<Int16VectorDataRecord,RSPN> DataRecordMakerTP<Int16VectorDataRecord,RSPN>::registerThis;
char SRKP[]="SRKP";
template const DataRecordMakerTP<Int16VectorDataRecord,SRKP> DataRecordMakerTP<Int16VectorDataRecord,SRKP>::registerThis;
char SVER[]="SVER";
template const DataRecordMakerTP< PascalStringDataRecord ,SVER> DataRecordMakerTP<PascalStringDataRecord,SVER>::registerThis; 
char TRKI[]="TRKI";
template const DataRecordMakerTP<Int16VectorDataRecord,TRKI> DataRecordMakerTP<Int16VectorDataRecord,TRKI>::registerThis;
char User[]="User";
template const DataRecordMakerTP<PascalStringDataRecord,User> DataRecordMakerTP<PascalStringDataRecord,User>::registerThis; 



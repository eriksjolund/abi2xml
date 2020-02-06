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

#ifndef PARAMS_H
#define PARAMS_H

#include <qtextcodec.h>

struct Params
{
  QTextCodec *codec;
  bool int_vector_as_attribute;
};

#endif

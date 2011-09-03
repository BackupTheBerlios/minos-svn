/*
 * Project: SDRMAXIV: A GUI Application for the QS1R Software Defined Radio
 *    
 * Copyright (C) 2011, Philip A Covington, N8VB
 * Software Radio Laboratory LLC http://www.srl-llc.com 
 * The author can be reached at: p.covington@gmail.com
 * 
 * This file is part of SDRMAXIV.
 *
 * SDRMAXIV is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * SDRMAXIV is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with SDRMAXIV. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

//#include <QVector>

enum QSDEMODMODE {
    dmAM = 0,
    dmSAM = 1,
    dmFMN = 2,
    dmFMW = 3,
    dmDSB = 4,
    dmLSB = 5,
    dmUSB = 6,
    dmCW = 7,
    dmDIG = 8
    };

enum QSAGCMODE {
    agcOFF = 0,
    agcLONG = 1,
    agcSLOW = 2,
    agcMED = 3,
    agcFAST = 4
          };

#define NUMBER_OF_RECEIVERS 1

#define RX1_CMD_SERV_TCP_PORT 43065
#define RX2_CMD_SERV_TCP_PORT 43066
#define RX1_DATA_TCP_PORT 43067
#define RX2_DATA_TCP_PORT 43068
#define WB_DATA_TCP_PORT 43069

#define RX1_CMD_SERV_UDP_TX_PORT 43070
#define RX2_CMD_SERV_UDP_TX_PORT 43071
#define RX1_CMD_SERV_UDP_RX_PORT 43072
#define RX2_CMD_SERV_UDP_RX_PORT 43073


#define SPECTRUM_INET_PACKET_MARKER_VER1 0x8ef3b285
#define SPECTRUM_LOCAL_PACKET_MARKER_VER1 0x20B4D33D
#define SPECTRUM_INET_EXTENDED_PACKET_MARKER_VER1 0x5ef3b978
#define SPECTRUM_LOCAL_EXTENDED_PACKET_MARKER_VER1 0x15B4D32D

#define WBDATA_INET_PACKET_MARKER_VER1 0x9ec3b27C
#define WBDATA_LOCAL_PACKET_MARKER_VER1 0x41B4E33A

#define GUI_VERSION "4.0.1.5 Beta 1"

#define SPEC_OFFSET_VALUE 200.0


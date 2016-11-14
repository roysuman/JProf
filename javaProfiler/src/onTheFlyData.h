/*
 * =====================================================================================
 *
 *       Filename:  onTheFlyData.h
 *
 *       Description:  
 *
 *       Version:  1.0
 *       Created:  Monday 14 November 2016 11:06:58  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *       Organization:  OPEN SOURCE
 *       LICENSE: GNU GPL
 *  
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * =====================================================================================
 */
#ifndef ON_THE_FLY_DATA_H
#define ON_THE_FLY_DATA_H

#define MAX_FRAME_TO_CAPTURE 128

typedef struct onTheFlyCallFrame_ onTheFlyCallFrame;
struct onTheFlyCallFrame_{
	size_t     frameNo;
	int64_t    methodId;
	std::string fileName;
	std::string className;
	std::string methodName;
};
// call trace copied from old .h file and renamed

typedef struct {
	int64_t threadId;                   // Env where trace was recorded
	size_t numFrames;                  // number of frames in this trace
	onTheFlyCallFrame frames[MAX_FRAME_TO_CAPTURE];          // frames
}callTrace;
#endif

/* Copyright (C) 
* 2012 - Michael.Kang blackfin.kang@gmail.com
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
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file arm_dyncom_memory.h
* @brief 
* @author Michael.Kang blackfin.kang@gmail.com
* @version 7849
* @date 2012-05-29
*/
#ifndef __ARM_DYNCOM_MEMORY_H__
#define __ARM_DYNCOM_MEMORY_H__
void memory_read(cpu_t* cpu, BasicBlock*bb, Value* addr, uint32_t sign, uint32_t size);
void memory_write(cpu_t* cpu, BasicBlock*bb, Value* addr, Value* value, uint32_t size);
#endif

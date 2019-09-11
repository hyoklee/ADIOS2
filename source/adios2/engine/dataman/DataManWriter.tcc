/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * DataManWriter.h
 *
 *  Created on: Jan 10, 2017
 *      Author: Jason Wang
 */

#ifndef ADIOS2_ENGINE_DATAMAN_DATAMAN_WRITER_TCC_
#define ADIOS2_ENGINE_DATAMAN_DATAMAN_WRITER_TCC_

#include "DataManWriter.h"

#include "adios2/common/ADIOSMPI.h"
#include "adios2/helper/adiosFunctions.h" //GetType<T>

#include <iostream>

namespace adios2
{
namespace core
{
namespace engine
{

template <class T>
void DataManWriter::PutSyncCommon(Variable<T> &variable, const T *values)
{
    PutDeferredCommon(variable, values);
    PerformPuts();
}

template <class T>
void DataManWriter::PutDeferredCommon(Variable<T> &variable, const T *values)
{

    variable.SetData(values);
    if (m_IsRowMajor)
    {
        m_DataManSerializer.PutVar(variable, m_Name, CurrentStep(), m_MpiRank,
                                   "", Params());
    }
    else
    {
        Dims start = variable.m_Start;
        Dims count = variable.m_Count;
        Dims shape = variable.m_Shape;
        Dims memstart = variable.m_MemoryStart;
        Dims memcount = variable.m_MemoryCount;
        std::reverse(start.begin(), start.end());
        std::reverse(count.begin(), count.end());
        std::reverse(shape.begin(), shape.end());
        std::reverse(memstart.begin(), memstart.end());
        std::reverse(memcount.begin(), memcount.end());
        m_DataManSerializer.PutVar(variable.m_Data, variable.m_Name, shape,
                                   start, count, memstart, memcount, m_Name,
                                   CurrentStep(), m_MpiRank, "", Params());
    }

    if (m_Reliable)
    {
        auto var = m_WriterSubIO.InquireVariable<T>(variable.m_Name);
        if (not var)
        {
            var = m_WriterSubIO.DefineVariable<T>(
                variable.m_Name, variable.m_Shape, variable.m_Start,
                variable.m_Count);
        }
        m_WriterSubEngine.Put<T>(var, values, adios2::Mode::Deferred);
    }
}

template <class T>
void DataManWriter::ReadVarFromFile(const std::string &varName)
{
}

} // end namespace engine
} // end namespace core
} // end namespace adios2

#endif /* ADIOS2_ENGINE_DATAMAN_DATAMAN_WRITER_TCC_ */

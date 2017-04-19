/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * IO.h
 *
 *  Created on: Feb 2017
 *      Author: Norbert Podhorszki
 */

#ifndef IO_H_
#define IO_H_

#include "adios2/HeatTransfer.h"
#include "adios2/Settings.h"

#include <mpi.h>

class IO
{
public:
    IO(const Settings &s, MPI_Comm comm);
    ~IO();
    void write(int step, const HeatTransfer &ht, const Settings &s,
               MPI_Comm comm);

private:
    std::string m_outputfilename;
};

#endif /* IO_H_ */

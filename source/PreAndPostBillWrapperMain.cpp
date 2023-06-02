#include "PreAndPostBillWrapper.h"
/*********************************************************************
#   File        :   PreAndPostBillWrapperMain.cpp
#   Abstract    :   Implementation of Pre and post bill check wrapper.
#
#   Date        :   18th Jul, 2013
#   Author(s)   :   Srinivasan V
#   Code Size   :   lines
#   Design      :
#   References  :
#   Module      :  
======================================================================
#   Revision History
======================================================================
#   SI     Edited by       Date           Area Edited
====================================================================== */
 
ABL_String m_execName;
int main(int argc, char** argv)
{
    ABL_String l_runMode =" ";
    ABL_String l_runType = " "; //'P're or p'O'st
    ABL_String l_confFile;
    //Check the commmand line argument.
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << "[S/N] [P/O]" << std::endl;
        return 1;
    }

    //Get the run mode from the 1st argument
     l_runType[0] = toupper(argv[1][0]);

    if(!(l_runType == "S" || l_runType == "N"))
    {
        std::cerr << "Usage: " << argv[0] << "[S/N] [P/O]" << std::endl;
        return 1;
    }

     
    //Get the run type from the 2nd argument
     l_runMode[0] = toupper(argv[2][0]);

    if(!(l_runMode == "P" || l_runMode == "O"))
    {
        std::cerr << "Usage: " << argv[0] << "[S/N] [P/O]" << std::endl;
        return 1;
    }

    if(l_runMode == "P")
    {
        m_execName = "PREBILL";
    }
    else
    {
        m_execName = "POSTBILL";
    }

    l_confFile = "../conf/PreAndPostBillWrapper.conf";

    //std::cerr<<l_confFile.c_str()<<std::endl;
    
    PreAndPostBillWrapper l_preAndPostBillWrapper(l_runType,l_runMode,l_confFile);
    if (!l_preAndPostBillWrapper.mb_init())
    {
        std::cerr<<"Initialization failed"<<std::endl;
        return 1;
    }
    l_preAndPostBillWrapper.mb_process();
    return 0;
}
/*********************************************************************
#
#                 Copyright 2012 - 2013  
#                 Tecnotree Convergence Ltd
#
 *********************************************************************/

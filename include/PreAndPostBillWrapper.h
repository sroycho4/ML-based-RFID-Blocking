#ifndef PREANDPOSTBILLWRAPPER_H
#define PREANDPOSTBILLWRAPPER_H

#include "OrchMessenger.h"
#include "ABL_Service.h"
#include "MessageQueue.h"

/*********************************************************************
#  File        :  PreAndPostBillWrapper.h
#  Abstract    :  Class to call the pre and post bill cheack procedure.
#  Date        :  31st May 2013
#  Author(s)   :  Srinivasan V
#  Code Size   :  lines
#  Design      :
#  References  :
#  Module      :  Commom
======================================================================
#   Revision History
======================================================================
#   SI    Edited by        Date                 Area Edited
======================================================================
 **********************************************************************/

class PreAndPostBillWrapper
{

    ABL_String m_confFile;
    ABL_Service *m_ABL_ServicePtr;
    ABL_String m_billDbID;
    ABL_Connection *m_billDbConnectionPtr;

    ABL_String m_inputQueue;
    ABL_String m_currentBillRunType;
    ABL_String m_currentBillRunMode;
    std::vector<ABL_String> m_vecOfTags;
    OrchMessenger *m_orchMessengerPtr;
    ABL_String m_execName;

    MessageRepository* m_msgToOrchestrator;// repository to send message
    MessageRepository* m_msgFromOrchestrator;    //repository to read Message


    //Will be used by BatchPrep for getting the input and output queues
    ABL_String m_receiveMessageFormat; //Receive the batch preparation request from task allocator at batch prep Q
    ABL_String m_batchCountMessageFormat; //Send the status of the batch to dispatcher Q
    ABL_String m_statsUpdatorMessageFormat; //Send the metrics of the batch to stats updator
    ABL_String m_taskAlMessageFormat; //Send the Task al
    ABL_String m_pauseActionMessageFormat; //Send the PAUSE ACK 

public:
    PreAndPostBillWrapper();
    PreAndPostBillWrapper(ABL_String p_currentBillRunType, ABL_String p_currentBillRunMode, ABL_String p_confFile);
    PreAndPostBillWrapper(const PreAndPostBillWrapper& p_preAndPostBillWrapper);
    virtual ~PreAndPostBillWrapper();
    bool mb_init();
    bool mb_process();
    bool mb_validRequest(ABL_String p_runMode, ABL_String p_billTypeInMsg);
};

#endif    /* PREANDPOSTBILLWRAPPER_H */
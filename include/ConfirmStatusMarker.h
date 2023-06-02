/** @file ConfirmStatusMarker.h
* @brief ConfirmStatusMarker for Confirmation
*/

//////////////////////////////////////////////////////////////////////
// ConfirmStatusMarker.h: Interface for Confirm Status Marker
//
//////////////////////////////////////////////////////////////////////


#ifndef CONF_STATUS_MARKER_H
#define CONF_STATUS_MARKER_H

#include "Base_Registry.h"
#include "ABL_String.h"
#include "ABL_Job.h"
#include "ABL_Service.h"
#include "ABL_CacheManager.h"
#include "CBaseComponent.h"
#include "Utils.h"

#include "ErrorDetails.h"
#include "ErrorCodes.h"
#include "BillJob.h"

#include "ConfigParams.h"
#include "ObjectManager.h"
#include "TransactionNumbers.h"


const ABL_String m_invoiceStatusUpdate = "InvoiceAccountStatusMarker";
const ABL_String m_serviceStatusUpdate = "ServiceAccountStatusMarker";

/*********************************************************************
#   File        :   ConfirmStatusMarker.h
#   Abstract    :   This component is used for Marking the status of the 
#                   Invoice and service accounts
#   Date        :   29th Aug 2013
#   Author(s)   :   Ralph
#   Code Size   :   
#   Design      :   
#   References  
#   Module      :   
======================================================================
#   Revision History
======================================================================
#   SI      Edited by       Date            Area Edited
======================================================================
#   1       Ralph          25-10-2013       Added Setter for Object Manager
                                            and modified error logging
======================================================================
#   Base Class  :   CBase
======================================================================
#   member 
#   Variables   :   
======================================================================
#   member 
#   Functions   :
#
======================================================================
Libraries: framework,components
**********************************************************************/

/** @class ConfirmStatusMarker
* @ingroup Components
* @brief Marks the accounts for error or success status
* @details 
*/

class EXPORT ConfirmStatusMarker : public CBaseComponent
{

protected:

    /** ABL_Service pointer
    */
    ABL_Service *m_ABL_ServicePtr; 

    /** MessageRepository Pointer
    */
    MessageRepository *m_messageRepository;

	/* Transaction number pointer*/
	TransactionNumbers *m_transactionNumbersPtr;

    int m_iprocessId;

    ObjectManager *m_objectManager;

    InvoiceAccountDetails::Iterator m_invIterator;
    InvoiceAccountDetails::Iterator m_invIteratorEnd;

    BillJob::Iterator m_billJobIterator;
    BillJob::Iterator m_billJobIteratorEnd;

    int m_iserviceAccCount;

    _int64 m_billCycleFullCode;             // BillCycleFull Code
    ABL_String m_additionalAction;          // Additional action


public:
    /** Default ConfirmStatusMarker default constructor
    */
    ConfirmStatusMarker();

    /** Default ConfirmStatusMarker default destructor
    */
    virtual ~ConfirmStatusMarker();

    /** Default ConfirmStatusMarker copy constructor
    */
    ConfirmStatusMarker(const ConfirmStatusMarker &p_ConfirmStatusMarker);

    virtual void mb_reset();

    /** Interface to clone the ConfirmStatusMarker object.
    */
    virtual Base_Registry* mb_clone(){return (Base_Registry*) new ConfirmStatusMarker(*this);}

	/** Interface for getting thread specific Object manager
	*/
	virtual void mb_setObjectManager(ObjectManager *p_objectManager) 
	{
		m_objectManager = p_objectManager;
	}

    /** This function is invoked by the framework.
    * @param p_servicePtr -> ABL_Service pointer provided by Framework
    * @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
    */
    virtual bool mb_initComponent(ABL_Service *p_servicePtr);


    /** This function is invoked by the framework
    * @param p_ABL_JobPtr ->  ABL_Job.Having the Processing Date.
    * @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
    */

    virtual bool mb_process(ABL_Job* &p_ABL_JobPtr);

    virtual bool mb_refreshComponent(MessageRepository *p_messageRepository,LocalCacheManager *p_localCacheManagerPtr);
    
    /** This function is invoked by the framework
    * @param p_ABL_CacheManagerPtr ->  ABL_CacheManager.
    * @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
    */
    virtual bool mb_getCache(ABL_CacheManager* p_ABL_CacheManagerPtr);

    /** Function to add processed charges to the Job.Should be implemented by General and Mock Components
    */
    virtual void mb_addCharge(BillJob *p_jobPtr,double p_accountCode,ABL_String p_chargeName,ProcessedCharge* p_processedChargePtr);

};//ConfirmStatusMarker

extern "C" {EXPORT Base_Registry* gb_getConfirmStatusMarker();}

#endif //CONF_STATUS_MARKER_H
/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



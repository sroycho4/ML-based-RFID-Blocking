/** @file CreditNote.h
* @brief Credit Note Component
*/

//////////////////////////////////////////////////////////////////////
// CreditNote.h: Interface for passing credit note
//
//////////////////////////////////////////////////////////////////////


#ifndef CREDIT_NOTE_H
#define CREDIT_NOTE_H

#include "Base_Registry.h"
#include "ABL_String.h"
#include "ABL_Job.h"
#include "ABL_Service.h"
#include "ABL_CacheManager.h"
#include "CBaseComponent.h"
#include "ErrorDetails.h"
#include "ErrorCodes.h"
#include "BillJob.h"
#include "ConfigParams.h"
#include "ObjectManager.h"
#include "Utils.h"
#include "TransactionNumbers.h"
#include "SubsRefundCharge.h"
#include <map>

/*********************************************************************
#   File        :   CreditNote.h
#   Abstract    :   This component is used for passing credit note
#   Date        :   20th Aug 2013
#   Author(s)   :   Ralph
#   Code Size   :   
#   Design      :   
#   References  
#   Module      :   Component
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

/** @class CreditNote
* @ingroup Components
* @brief Pass credit note
* @details 
*/

class EXPORT CreditNote : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service         *m_ABL_ServicePtr;

	ConfigParams        *m_configParams;

	ObjectManager       *m_objectManager;

	TransactionNumbers  *m_transNumbers;

	//Parameters from the Queue 
	_int64          m_billCycleFullCode;
	ABL_Date        m_cycleEndDate;
	ABL_String      m_additionalAction;

	//Insufficient transaction numbers flag
	bool            m_insufficientTransNosFlag;
	ABL_String      m_errorMsg;
	long            m_errorCode;

public:
	/** Default CreditNote default constructor
	*/
	CreditNote();

	/** Default CreditNote default destructor
	*/
	virtual ~CreditNote();

	/** Default CreditNote copy constructor
	*/
	CreditNote(const CreditNote &p_CreditNote);

	/** Interface to clone the CreditNote object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new CreditNote(*this);}

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
	virtual bool mb_initComponent(ABL_Service *p_servicePtr = NULL);


	/** This function is invoked by the framework
	* @param p_ABL_JobPtr ->  ABL_Job
	* @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
	*/

	virtual bool mb_process(ABL_Job* &p_ABL_JobPtr);

	/** This function is for implementing the stop logic...Called when framework receives stop signal
	* @param 
	* @return -> boolean -> True on successfully stopping the component
	*/
	virtual bool mb_stop();

	virtual bool mb_refreshComponent(MessageRepository *p_messageRepository,LocalCacheManager *p_localCacheManagerPtr);

	/** This function is invoked by the framework
	* @param p_ABL_CacheManagerPtr ->  ABL_CacheManager.
	* @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
	*/
	virtual bool mb_getCache(ABL_CacheManager* p_ABL_CacheManagerPtr);

	virtual void mb_addCharge(BillJob * p_jobPtr,double p_accountCode,ABL_String ChargeName,ProcessedCharge* p_processedChargePtr);

};//CreditNote

extern "C" {EXPORT Base_Registry* gb_getCreditNote();}

#endif //CREDIT_NOTE_H


/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



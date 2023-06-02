/** @file AdvancePayCreditNote.h
* @brief Credit Note from the paid advance amount
*/

//////////////////////////////////////////////////////////////////////
// AdvancePayCreditNote.h: Interface for passing credit note from paid advance
//
//////////////////////////////////////////////////////////////////////


#ifndef ADV_PAY_CREDIT_NOTE_H
#define ADV_PAY_CREDIT_NOTE_H

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
#   File        :   AdvancePayCreditNote.h
#   Abstract    :   This component is used for passing credit note from paid advance
#   Date        :   7th May 2013
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
#
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

/** @class AdvancePayCreditNote
* @ingroup Components
* @brief Pass credit note from paid advance
* @details 
*/

class EXPORT AdvancePayCreditNote : public CBaseComponent
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

	//Insufficient transaction numbers flag
	bool            m_insufficientTransNosFlag;
	ABL_String      m_errorMsg;
	long            m_errorCode;

public:
	/** Default AdvancePayCreditNote default constructor
	*/
	AdvancePayCreditNote();

	/** Default AdvancePayCreditNote default destructor
	*/
	virtual ~AdvancePayCreditNote();

	/** Default AdvancePayCreditNote copy constructor
	*/
	AdvancePayCreditNote(const AdvancePayCreditNote &p_AdvancePayCreditNote);

	/** Interface to clone the AdvancePayCreditNote object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new AdvancePayCreditNote(*this);}

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

};//AdvancePayCreditNote

extern "C" {EXPORT Base_Registry* gb_getAdvancePayCreditNote();}

#endif //ADV_PAY_CREDIT_NOTE_H


/*********************************************************************
#
#                 Copyright 2013 - 2014
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



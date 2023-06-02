/** @file DisablePrepaidBilling.h
* @brief DisablePrepaidBilling Component
*/

//////////////////////////////////////////////////////////////////////
// DisablePrepaidBilling.h: Interface for DisablePrepaidBilling
//
//////////////////////////////////////////////////////////////////////


#ifndef DISABLE_PREPAID_BILLING_H
#define DISABLE_PREPAID_BILLING_H

#include "Base_Registry.h"
#include "ABL_String.h"
#include "ABL_Job.h"
#include "ABL_Service.h"
#include "ABL_CacheManager.h"
#include "CBaseComponent.h"
#include "ErrorDetails.h"
#include "ErrorCodes.h"
#include "BillJob.h"
#include "ObjectManager.h"
#include "Utils.h"
#include <map>

/*********************************************************************
#   File        :   DisablePrepaidBilling.h
#   Abstract    :   This component is used for doing DisablePrepaidBilling
#   Date        :   15th Jul,2015
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

/** @class DisablePrepaidBilling
* @ingroup Components
* @brief Disables billing for prepaid accounts
* @details 
*/

class EXPORT DisablePrepaidBilling : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service         *m_ABL_ServicePtr;

	ObjectManager       *m_objectManager;

	//Parameters from the Queue 
	_int64              m_billCycleFullCode;
	ABL_Date            m_cycleEndDate;

public:
	/** Default DisablePrepaidBilling default constructor
	*/
	DisablePrepaidBilling();

	/** Default DisablePrepaidBilling default destructor
	*/
	virtual ~DisablePrepaidBilling();

	/** Default DisablePrepaidBilling copy constructor
	*/
	DisablePrepaidBilling(const DisablePrepaidBilling &p_DisablePrepaidBilling);

	/** Interface to clone the DisablePrepaidBilling object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new DisablePrepaidBilling(*this);}

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

	virtual bool mb_refreshComponent(MessageRepository *p_messageRepository,LocalCacheManager *p_localCacheManagerPtr);

	/** This function is invoked by the framework
	* @param p_ABL_CacheManagerPtr ->  ABL_CacheManager.
	* @return boolean -> True on successfully loading the data.Exceptions for any unexpected errors.
	*/
	virtual bool mb_getCache(ABL_CacheManager* p_ABL_CacheManagerPtr);

	virtual void mb_addCharge(BillJob * p_jobPtr,double p_accountCode,ABL_String ChargeName,ProcessedCharge* p_processedChargePtr);

};//DisablePrepaidBilling

extern "C" {EXPORT Base_Registry* gb_getDisablePrepaidBilling();}

#endif 


/*********************************************************************
#
#                 Copyright 2015 - 2016
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



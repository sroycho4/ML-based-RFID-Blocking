/** @file AutoDebit.h
* @brief AutoDebit Component
*/

//////////////////////////////////////////////////////////////////////
// AutoDebit.h: Interface for doing AutoDebit
//
//////////////////////////////////////////////////////////////////////


#ifndef AUTO_DEBIT_H
#define AUTO_DEBIT_H

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
#include <map>

/*********************************************************************
#   File        :   AutoDebit.h
#   Abstract    :   This component is used for doing AutoDebit
#   Date        :   18th Sep 2013
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

/** @class AutoDebit
* @ingroup Components
* @brief Pass credit note
* @details 
*/

class EXPORT AutoDebit : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service         *m_ABL_ServicePtr;

	ConfigParams        *m_configParams;

	ObjectManager       *m_objectManager;

	//Parameters from the Queue 
	_int64              m_billCycleFullCode;

public:
	/** Default AutoDebit default constructor
	*/
	AutoDebit();

	/** Default AutoDebit default destructor
	*/
	virtual ~AutoDebit();

	/** Default AutoDebit copy constructor
	*/
	AutoDebit(const AutoDebit &p_AutoDebit);

	/** Interface to clone the AutoDebit object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new AutoDebit(*this);}

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

};//AutoDebit

extern "C" {EXPORT Base_Registry* gb_getAutoDebit();}

#endif //AUTO_DEBIT_H


/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



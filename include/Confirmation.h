/** @file Confirmation.h
* @brief Confirmation Component
*/

//////////////////////////////////////////////////////////////////////
// Confirmation.h: Interface for making dunning entry for the subscriber
//
//////////////////////////////////////////////////////////////////////


#ifndef CONFIRMATION_H
#define CONFIRMATION_H

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
#   File        :   Confirmation.h
#   Abstract    :   This component for doing confirmation process
#   Date        :   23rd Oct 2013
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

/** @class Confirmation
* @ingroup Components
* @brief Pass credit note
* @details 
*/

class EXPORT Confirmation : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service         *m_ABL_ServicePtr;

	ConfigParams        *m_configParams;

	ObjectManager       *m_objectManager;

	//Parameters from the Queue 
	_int64          m_billCycleFullCode;
	ABL_Date        m_cycleStartDate;
	ABL_Date        m_cycleEndDate;
	ABL_String      m_billRunType;
	
	//Deduced parameters
	_int64          m_nextBillCycleFullCode;


public:
	/** Default Confirmation default constructor
	*/
	Confirmation();

	/** Default Confirmation default destructor
	*/
	virtual ~Confirmation();

	/** Default Confirmation copy constructor
	*/
	Confirmation(const Confirmation &p_Confirmation);

	/** Interface to clone the Confirmation object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new Confirmation(*this);}

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

};//Confirmation

extern "C" {EXPORT Base_Registry* gb_getConfirmation();}

#endif //CONFIRMATION_H


/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



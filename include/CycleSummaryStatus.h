/** @file CycleSummaryStatus.h
* @brief CycleSummaryStatus Component
*/

//////////////////////////////////////////////////////////////////////
// CycleSummaryStatus.h: Interface for doing CycleSummaryStatus
//
//////////////////////////////////////////////////////////////////////


#ifndef CYCLE_SUMMARY_STATUS_H
#define CYCLE_SUMMARY_STATUS_H

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
#   File        :   CycleSummaryStatus.h
#   Abstract    :   This component is used for doing CycleSummaryStatus
#   Date        :   05th Feb,2014
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

/** @class CycleSummaryStatus
* @ingroup Components
* @brief Cycle Summary Status
* @details 
*/

class EXPORT CycleSummaryStatus : public CBaseComponent
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
	/** Default CycleSummaryStatus default constructor
	*/
	CycleSummaryStatus();

	/** Default CycleSummaryStatus default destructor
	*/
	virtual ~CycleSummaryStatus();

	/** Default CycleSummaryStatus copy constructor
	*/
	CycleSummaryStatus(const CycleSummaryStatus &p_BillCycleChange);

	/** Interface to clone the CycleSummaryStatus object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new CycleSummaryStatus(*this);}

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

};//CycleSummaryStatus

extern "C" {EXPORT Base_Registry* gb_getCycleSummaryStatus();}

#endif //CYCLE_SUMMARY_STATUS_H


/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



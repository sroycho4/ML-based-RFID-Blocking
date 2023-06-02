/** @file InvoicePosting.h
* @brief Invoice Posting Component
*/

//////////////////////////////////////////////////////////////////////
// InvoicePosting.h: Interface to prepare for Invoice Posting
//
//////////////////////////////////////////////////////////////////////


#ifndef INV_POST_H
#define INV_POST_H

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
#   File        :   InvoicePosting.h
#   Abstract    :   This component is used to prepare for Invoice Posting
#   Date        :   11th Aug 2015
#   Author(s)   :   Shriraj
#   Code Size   :   
#   Design      :   
#   References  
#   Module      :   Component
======================================================================
#   Revision History
======================================================================
#   SI      Edited by       Date            Area Edited
======================================================================
#   1       Shriraj G.     11-08-2015       Initial Version
                                                                      
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

/** @class InvoicePosting
* @ingroup Components
* @brief prepare for posting
* @details 
*/

class EXPORT InvoicePosting : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service         *m_ABL_ServicePtr
		;
	ObjectManager       *m_objectManager;

    _int64 m_billCycleFullCode;

public:
	/** Default InvoicePosting default constructor
	*/
	InvoicePosting();

	/** Default InvoicePosting default destructor
	*/
	virtual ~InvoicePosting();

	/** Default InvoicePosting copy constructor
	*/
	InvoicePosting(const InvoicePosting &p_InvoicePosting);

	/** Interface to clone the InvoicePosting object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new InvoicePosting(*this);}

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

};//Dunning

extern "C" {EXPORT Base_Registry* gb_getInvoicePosting();}

#endif //INV_POST_H


/*********************************************************************
#
#                 Copyright 2015 - 2016
#                 Tecnotree Convergence Pvt. Ltd.
#
*********************************************************************/

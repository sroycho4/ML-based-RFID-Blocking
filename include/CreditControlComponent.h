/** @file CreditControlComponent.h
* @brief CreditControlComponent Component
*/

//////////////////////////////////////////////////////////////////////
// CreditControlComponent.h: Interface for making CreditControl entry for the service
//
//////////////////////////////////////////////////////////////////////


#ifndef CREDIT_CONTROL_COMPONENT_H
#define CREDIT_CONTROL_COMPONENT_H

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

#include "SubServiceParamsCache.h"

#include <map>

/*********************************************************************
#   File        :   CreditControlComponent.h
#   Abstract    :   This component is used for making credit control entry for the service code
#   Date        :   23rd Mar, 2015
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

/** @class CreditControlComponent
* @ingroup Components
* @brief Makes an entry for crdit control schedule to be triggered for the account
* @details 
*/

class EXPORT CreditControlComponent : public CBaseComponent
{

protected:

    /** ABL_Service pointer
    */
    ABL_Service         *m_ABL_ServicePtr;

    ConfigParams        *m_configParams;

    ObjectManager       *m_objectManager;

    /*Define a data_container for SubServiceParamsCache*/
    data_container      *m_SubServiceParamsCacheContainer;

    /* Define a Search Object for SubServiceParamsCache*/
    SubServiceParamsCache   *m_SubServiceParamsSearchObj;

    //Configuration params
    ABL_String           m_accountLevelCreditControlFlag;

public:
    /** Default CreditControlComponent default constructor
    */
    CreditControlComponent();

    /** Default CreditControlComponent default destructor
    */
    virtual ~CreditControlComponent();

    /** Default CreditControlComponent copy constructor
    */
    CreditControlComponent(const CreditControlComponent &p_CreditControlComponent);

    /** Interface to clone the CreditControlComponent object.
    */
    virtual Base_Registry* mb_clone(){return (Base_Registry*) new CreditControlComponent(*this);}

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

};//CreditControlComponent

extern "C" {EXPORT Base_Registry* gb_getCreditControlComponent();}

#endif //CREDIT_CONTROL_COMPONENT_H


/*********************************************************************
#
#                 Copyright 2014 - 2015
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



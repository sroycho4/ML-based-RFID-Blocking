/** @file TaxCalculation.h
* @brief Tax Calculation Component
*/

//////////////////////////////////////////////////////////////////////
// TaxCalculation.h: Interface for the Calculating the Tax.
//
//////////////////////////////////////////////////////////////////////


#ifndef TAX_CALC_H
#define TAX_CALC_H

#include "Base_Registry.h"
#include "ABL_String.h"
#include "ABL_Job.h"
#include "ABL_Service.h"
#include "ABL_CacheManager.h"
#include "CBaseComponent.h"
#include "TaxCharge.h"
#include "CalculatedCharge.h"

#include "ErrorDetails.h"
#include "ErrorCodes.h"
#include "BillJob.h"
#include "InvoiceAccDetails.h"
#include "ServiceAccountDetails.h"

#include "ProfitCenterCodeAndGLSelector.h"
#include "PackageCache.h"
#include "ArticleCache.h"
#include "ProfitCostCenterCache.h"
#include "ArticleGLCodesCache.h"
#include "ControlArticlesCache.h"

#include "TaxCache.h"
#include "CurrencyCache.h"
#include "ConfigParams.h"
#include "ObjectManager.h"
#include "Utils.h"

#include <map>
#include <utility>


/*********************************************************************
#   File        :   TaxCalculation.h
#   Abstract    :   This component is used for Calculating the Tax while posting credit notes
#   Date        :   23rd Aug 2013
#   Author(s)   :   Ralph
#   Code Size   :   
#   Design      :   
#   References  
#   Module      :   Components
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

/** @class TaxCalculation
* @ingroup Components
* @brief Calculates the Tax for all the amounts under an invoice
* @details Apart from OverallTax, this component calculates liable tax part
*/

class EXPORT TaxCalculation : public CBaseComponent
{

protected:

	/** ABL_Service pointer
	*/
	ABL_Service    *m_ABL_ServicePtr; 

	/*Define a data_container for Package*/
	data_container *m_packageContainer;

	/*Define a data_container for Articles*/
	data_container *m_articleContainer;

	/*Define a data_container for ProfitCostCenterCache*/
	data_container *m_profitCenterContainer;

	/*Define a data_container for TaxPlanDetails*/
	data_container *m_taxContainer;

	/*Define a data_container for CurrencyCache*/
	data_container *m_currencyContainer;

	/*Define a data_container for ArticleGLCodesCache*/
	data_container *m_articlesGlCodesContainer;

	/*Define a data_container for Control articles*/
	data_container *m_controlArticlesContainer;

	/* Define a Search Object for Services*/
	PackageCache   *m_packageSearchObj;

	/* Define a Search Object for Articles*/
	ArticleCache   *m_articleCacheSearchObj;

	/* Define a Search Object for ProfitCostCenterCache*/
	ProfitCostCenterCache   *m_profitCenterSearchObj;

	/* Define a Search Object for ArticleGLCodesCache*/
	ArticleGLCodesCache     *m_articlesGlCodesSearchObj;

	/* Define a Search Object for TaxCache*/
	TaxCache       *m_taxSearchObj;

	/* Define a Search Object for CurrencyCache*/
	CurrencyCache  *m_currencyCacheSearchObj;

	/* Define a Search Object for Control articles*/
	ControlArticlesCache   *m_controlArticlesSearchObj;

	ConfigParams   *m_configParams;

	ObjectManager  *m_objectManager;

	ProfitCenterCodeAndGLSelector* m_profitCentreGlSelectorPtr;

	//Configuration Param values
	ABL_String      m_nilTaxPlanCode;

	//Flag to indicate if the currency conversion is needed for the charge
	ABL_String      m_currencyConversionFlag;

	//Parameters from the Queue 
	_int64          m_billCycleFullCode;

	//Liable tax parameters

	/* Liable tax article code */
	ABL_String      m_liableTaxArticleCode;

	/* Liable tax article availability flag*/
	bool            m_liableTaxArticleAvailableFlg;

	/*Liable Tax Article details*/
	ArticleCache*   m_liableTaxArticleDetails;


	//Exchange rate
	double          m_dexchangeRate;

	//Exchange multiplication factor
	double          m_dexchangeMultplicationFactor;

	void            mb_updateCalculatedChargeCurrencyFields(CalculatedCharge* p_calculatedCharge);

	double          mb_calculateTaxAmt(double p_artAmt, ABL_String p_taxCode , ABL_String &p_taxCalculationStore);

	void            mb_getLiableTaxArticleDetails();

public:
	/** Default TaxCalculation default constructor
	*/
	TaxCalculation();

	/** Default TaxCalculation default destructor
	*/
	virtual ~TaxCalculation();

	/** Default TaxCalculation copy constructor
	*/
	TaxCalculation(const TaxCalculation &p_TaxCalculation);

	/** Interface to clone the TaxCalculation object.
	*/
	virtual Base_Registry* mb_clone(){return (Base_Registry*) new TaxCalculation(*this);}

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

	virtual void mb_addCharge(BillJob * p_jobPtr,double p_accountCode,ABL_String ChargeName,ProcessedCharge* p_processedChargePtr);

};//TaxCalculation

extern "C" {EXPORT Base_Registry* gb_getTaxCalculation();}

#endif //TAX_CALC_H


/*********************************************************************
#
#                 Copyright 2012 - 2013
#                 Tecnotree Convergence Ltd.
#
*********************************************************************/



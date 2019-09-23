/*
 * Author : Akshay S Kulkarni
 * Demonstrates SIM TEST
 * Copyright (C) Syratron Technologies Pvt.Ltd Banglore
 *
 */

#include "legato.h"
#include "interfaces.h" // TO INCLUDE ALL LIBRARIES FROM LEGATO SERVICES Example : le_sim_States_t ,le_sim_GetState()

//GLOBAL VARIABLES

le_sim_States_t state;
le_sim_Id_t simID=1;

//-------------------------------- GET NETWORK REGISTRATION STATE ----------------------- //

int GetNwRegState()

{

le_mrc_NetRegState_t netstate;
le_mrc_GetNetRegState (&netstate);
	if(netstate==LE_MRC_REG_HOME)
		{
//			LE_INFO("REGISTERED TO HOME N/W");
			return 1;
		}
	else if(netstate==LE_MRC_REG_NONE)
		{
//			LE_INFO("NOT REGISTERED\n");
			return 0;
		}
	else if(netstate==LE_MRC_REG_SEARCHING)
		{
//			LE_INFO("SEARCHING\n");
			return 2;
		}
	else if(netstate==LE_MRC_REG_DENIED)
		{
//			LE_INFO("REGISTRAION DENIED\n");
			return 3;
		}
	else if(netstate==LE_MRC_REG_ROAMING)
		{
//			LE_INFO("ROAMING REGISTERED\n");
			return 4;
		}
	else if(netstate==LE_MRC_REG_UNKNOWN)
		{
//			LE_INFO("REGISTRATION UNKNOWN \n");
			return 5;
		}
	else
		{
			LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING N/W REGISTRATION STATE \n");
			return -1;
		}
}



//-------------------------------- GET RAT / RADIO ACCESS TECHNOLOGY ----------------------- //

int get_rat()
{
	le_mrc_MetricsRef_t metricsRef = le_mrc_MeasureSignalMetrics();
	le_mrc_Rat_t rat;
	rat = le_mrc_GetRatOfSignalMetrics(metricsRef);
	if(rat == LE_MRC_RAT_GSM)
	{
		LE_INFO("RAT is GSM \n");
		return 1;
	}
	else if(rat == LE_MRC_RAT_LTE)
	{
		LE_INFO("RAT is LTE \n");
		return 2;
	}
	else
	{
		LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING THE RAT \n");
		return -1;
	}
}


//---------------------------------GET LTE SIGNAL METRICS  -------------------------//

void get_ltesignalstrength()
{
le_result_t result;
le_mrc_MetricsRef_t MetricsRef = le_mrc_MeasureSignalMetrics();
int32_t ssPtr=0; // Signal strength in DBM
uint32_t blerPtr=0; // Block error rate
int32_t rsrqPtr=0; // rsrq value in dB as measured
int32_t rsrpPtr=0; //Current RSRP in dBm
int32_t sinrPtr=0; // SNR Level in dB with 1 decimal
result = le_mrc_GetLteSignalMetrics(MetricsRef,&ssPtr,&blerPtr,&rsrqPtr,&rsrpPtr,&sinrPtr);
	if (result == LE_OK)
		{
			LE_INFO("LTE SIGNAL STRENGTH IN dBm is : %d & Block error rate is : %d & RSRQ value is %d & RSRP value is %d and SNR level is %d\n",ssPtr,blerPtr,rsrqPtr,rsrpPtr,sinrPtr);
		}
	else
		{
			LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING GSM SIGNAL STRENGTH PLEASE TRY AGAIN\n");
		}
}


//---------------------------------GET GSM SIGNAL METRICS  -------------------------//

void get_gsmsignalstrength()
{
le_result_t result;
le_mrc_MetricsRef_t MetricsRef = le_mrc_MeasureSignalMetrics();
int32_t RssiVal=0; // Signal strength in DBM
uint32_t BerVal=0; // Bit error rate
result = le_mrc_GetGsmSignalMetrics(MetricsRef,&RssiVal,&BerVal);
	if (result == LE_OK)
		{
			LE_INFO("GSM SIGNAL STRENGTH IN dBm is : %d & Bit error is : %d\n",RssiVal,BerVal);
		}
	else
		{
			LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING GSM SIGNAL STRENGTH PLEASE TRY AGAIN\n");
		}
}

//---------------------------------GET PHONE NUMBER  -------------------------//
void get_phoneNumber()
{
	char number[50];
	le_result_t result;
	result=le_sim_GetSubscriberPhoneNumber(simID,number,sizeof(number));
	if(result == LE_OK)
		{
			LE_INFO("PHONE NUMBER : %s",number);
		}
		else
		{
			LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING PHONE NUMBER PLEASE TRY AGAIN\n");
		}

}



//---------------------------------GET OPERATOR INFO -------------------------//

void get_simOperator()
{
	char name[50];
	le_result_t result;
	result = le_sim_GetHomeNetworkOperator(simID,name,sizeof(name));
	if(result == LE_OK)
	{
		LE_INFO("SIM OPERATOR NAME : %s",name);
	}
	else
	{
		LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING SIM OPERATOR PLEASE TRY AGAIN\n");
	}
}

//---------------------------------GET SIM STATE -------------------------//

void get_simstate()
{
	int ret1,ret2;
	// CHECK WHETHER SIM IS READY OR NO
			 state = le_sim_GetState(simID);
			 if (state == LE_SIM_READY || state == LE_SIM_INSERTED)
			 		 {
			 			 LE_INFO("SIM IS INSERTED & READY \n");
			 			 sleep(2);
			 			 LE_INFO("FETCHING SIM OPERATOR INFO \n");
			 			 get_simOperator();
			 			LE_INFO("FETCHING PHONE NUMBER \n");
			 			get_phoneNumber();
			 			LE_INFO("FETCHING RAT\n");
			 			ret1 = get_rat();
			 			if(ret1==1)
			 			{
			 			LE_INFO("FETCHING GSM SIGNAL STRENGTH \n");
			 			get_gsmsignalstrength();
			 			}
			 			else if(ret1==2)
			 			{
			 			LE_INFO("FETCHING LTE SIGNAL STRENGTH \n");
			 			get_ltesignalstrength();
			 			}
			 			LE_INFO("FETCHING NETWORK REGISTRATION STATE \n");
			 			ret2=GetNwRegState();
			 			if(ret2==0)
			 			{
			 				LE_INFO("NOT REGISTERED\n");
			 			}
			 			else if(ret2==1)
			 			{
			 				LE_INFO("REGISTERED TO HOME N/W");
			 			}
			 			else if (ret2==2)
			 			{
			 				LE_INFO("SEARCHING\n");
			 			}
			 			else if (ret2==3)
			 			{
			 				LE_INFO("REGISTRAION DENIED\n");
			 			}
			 			else if (ret2==4)
			 			{
			 				LE_INFO("ROAMING REGISTERED\n");
			 			}
			 			else if (ret2==5)
			 			{
			 				LE_INFO("REGISTRATION UNKNOWN \n");
			 			}
			 			else
			 			{
			 				LE_INFO("OOPS !!! SOMETHING WENT WRONG IN KNOWING N/W REGISTRATION STATE \n");
			 			}
			 		 }
			 else if (state == LE_SIM_ABSENT)
			         {
			        	 LE_INFO("SIM IS ABSENT \n");
			         }
			 else if (state == LE_SIM_BLOCKED)
			         {
			        	 LE_INFO("SIM IS BLOCKED \n");
			         }
			 else if (state == LE_SIM_BUSY)
			         {
			        	 LE_INFO("SIM IS BUSY \n");
			         }
	         else if (state == LE_SIM_POWER_DOWN)
			         {
			        	 LE_INFO("SIM POWER DOWN \n");
			         }
		     else if (state == LE_SIM_STATE_UNKNOWN)
			         {
			        	 LE_INFO("SIM STATE IS UNKNOWN \n");
			         }
			 else
			         {
			        	 LE_INFO("OOPS!!!! SOMETHING WENT WRONG PLEASE TRY AGAIN\n");
			         }

}

//--------------------------------- COMPONENT INITIALIZATION / MAIN FUNCTION -------------------------//

COMPONENT_INIT
{
    LE_INFO("WELCOME TO SIM-TEST SAMPLE CODE");
    get_simstate();
    LE_INFO("SIM TEST COMPLETED");

}
